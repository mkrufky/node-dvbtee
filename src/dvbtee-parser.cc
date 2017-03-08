#include <nan.h>
#include "dvbtee-parser.h"

TableData::TableData(const uint8_t &tableId, const std::string &decoderName, const std::string &json)
: tableId(tableId)
, decoderName(decoderName)
, json(json)
{}

////////////////////////////////////////

TableReceiver::TableReceiver(dvbteeParser *dvbteeParser)
: m_dvbteeParser(dvbteeParser)
{
  uv_mutex_init(&m_ev_mutex);
  uv_async_init(uv_default_loop(), &m_async, completeCb);
  m_async.data = this;
}

TableReceiver::~TableReceiver()
{
  unregisterInterface();

  m_cb.Reset();

  uv_mutex_lock(&m_ev_mutex);
  while (!m_eq.empty())
  {
    TableData *data = m_eq.front();
    delete data;
    m_eq.pop();
  }
  uv_mutex_unlock(&m_ev_mutex);

  uv_mutex_destroy(&m_ev_mutex);
}

void TableReceiver::subscribe(const v8::Local<v8::Function> &fn)
{
  m_cb.SetFunction(fn);
  registerInterface();
}

void TableReceiver::registerInterface()
{
  m_dvbteeParser->m_parser.subscribeTables(this);
}

void TableReceiver::unregisterInterface()
{
  m_dvbteeParser->m_parser.subscribeTables(NULL);
}

void TableReceiver::updateTable(uint8_t tId, dvbtee::decode::Table *table)
{
  uv_mutex_lock(&m_ev_mutex);
  m_eq.push(new TableData(table->getTableid(), table->getDecoderName(), table->toJson()));
  uv_mutex_unlock(&m_ev_mutex);

  uv_async_send(&m_async);
}

#if defined(NODE_MAJOR_VERSION) && (NODE_MAJOR_VERSION == 0 && \
    defined(NODE_MINOR_VERSION) && (NODE_MINOR_VERSION < 12))

// based on http://stackoverflow.com/a/30897476/5535550
v8::Local<v8::Value> v8_JSON_Parse(v8::Local<v8::Value> jsonString) {

  v8::Local<v8::Context> context = Nan::GetCurrentContext();
  v8::Local<v8::Object> global = context->Global();

  // find "JSON" object in global scope
  v8::Local<v8::Value> jsonValue = global->Get(Nan::New("JSON").ToLocalChecked());

  if (!jsonValue->IsObject()) {
    return Nan::Undefined();
  }

  v8::Local<v8::Object> json = jsonValue->ToObject();

  // find "parse" attribute
  v8::Local<v8::Value> parse = json->Get(Nan::New("parse").ToLocalChecked());

  if (parse.IsEmpty() || !parse->IsFunction()) {
    return Nan::Undefined();
  }

  // cast into a function
  v8::Local<v8::Function> parseFunction = v8::Local<v8::Function>::Cast(parse);

  // and call it
  return parseFunction->Call(json, 1, &jsonString);
}
#else
#define v8_JSON_Parse v8::JSON::Parse
#endif

void TableReceiver::notify()
{
  Nan::HandleScope scope;
  uv_mutex_lock(&m_ev_mutex);
  while (!m_eq.empty())
  {
    TableData *data = m_eq.front();
    uv_mutex_unlock(&m_ev_mutex);

    if (!m_cb.IsEmpty()) {

      v8::Local<v8::String> jsonStr = Nan::New(data->json).ToLocalChecked();
      v8::Local<v8::Value> jsonObj = v8_JSON_Parse(jsonStr);

      v8::Local<v8::Value> argv[] = {
        Nan::New(data->tableId),
        Nan::New(data->decoderName).ToLocalChecked(),
        jsonObj
      };

      m_cb.Call(3, argv);
    }

    delete data;

    uv_mutex_lock(&m_ev_mutex);
    m_eq.pop();
  }
  uv_mutex_unlock(&m_ev_mutex);
}

void TableReceiver::completeCb(uv_async_t *handle
#if NODE_MODULE_VERSION<=11
                              , int status /*UNUSED*/
#endif
                              )
{
  TableReceiver* rcvr = static_cast<TableReceiver*>(handle->data);
  rcvr->notify();
}

////////////////////////////////////////

Nan::Persistent<v8::Function> dvbteeParser::constructor;

dvbteeParser::dvbteeParser()
: m_tableReceiver(this)
{
}

dvbteeParser::~dvbteeParser() {
}

void dvbteeParser::Init(v8::Handle<v8::Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Parser").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "reset",        reset);
  Nan::SetPrototypeMethod(tpl, "feed",         feed);
  Nan::SetPrototypeMethod(tpl, "push",         feed); // deprecated
  Nan::SetPrototypeMethod(tpl, "parse",        feed); // deprecated
  Nan::SetPrototypeMethod(tpl, "listenTables", listenTables);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Parser").ToLocalChecked(), tpl->GetFunction());
}

void dvbteeParser::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new dvbteeParser(...)`
    dvbteeParser* obj = new dvbteeParser();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    // Invoked as plain function `dvbteeParser(...)`, turn into construct call.
    const int argc = 0;
    v8::Local<v8::Value> argv[argc] = { };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void dvbteeParser::listenTables(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dvbteeParser* obj = ObjectWrap::Unwrap<dvbteeParser>(info.Holder());

  int lastArg = info.Length() - 1;

  if ((lastArg >= 0) && (info[lastArg]->IsFunction())) {
    obj->m_tableReceiver.subscribe(info[lastArg].As<v8::Function>());
  }

  info.GetReturnValue().Set(info.Holder());
}

////////////////////////////////////////

class ResetWorker : public Nan::AsyncWorker {
public:
  ResetWorker(Nan::Callback *callback, const Nan::FunctionCallbackInfo<v8::Value>& info)
    : Nan::AsyncWorker(callback)
    {
      m_obj = Nan::ObjectWrap::Unwrap<dvbteeParser>(info.Holder());
    }
  ~ResetWorker()
    {
    }

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute () {
    m_obj->m_parser.cleanup();
    m_obj->m_parser.reset();
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback () {
    Nan::HandleScope scope;

    v8::Local<v8::Value> argv[] = {
        Nan::Null()
      , Nan::New<v8::Number>(0)
    };

    callback->Call(2, argv);
  }

private:
  dvbteeParser* m_obj;
};

void dvbteeParser::reset(const Nan::FunctionCallbackInfo<v8::Value>& info) {

  // If a callback function is supplied, this method will run async
  // otherwise, it will run synchronous

  int lastArg = info.Length() - 1;

  if ((lastArg >= 0) && (info[lastArg]->IsFunction())) {

    Nan::Callback *callback = new Nan::Callback(info[lastArg].As<v8::Function>());
    Nan::AsyncQueueWorker(new ResetWorker(callback, info));

  } else {
    dvbteeParser* obj = ObjectWrap::Unwrap<dvbteeParser>(info.Holder());

    obj->m_parser.cleanup();
    obj->m_parser.reset();

    info.GetReturnValue().Set(info.Holder());
  }
}

////////////////////////////////////////

class FeedWorker : public Nan::AsyncWorker {
public:
  FeedWorker(Nan::Callback *callback, const Nan::FunctionCallbackInfo<v8::Value>& info)
    : Nan::AsyncWorker(callback)
    , m_buf(NULL)
    , m_buf_len(0)
    , m_ret(-1)
    {
      m_obj = Nan::ObjectWrap::Unwrap<dvbteeParser>(info.Holder());

      if ((info[0]->IsObject()) && (info[1]->IsNumber())) {

        const char *key = "buf";
        SaveToPersistent(key, info[0]);
        m_buf = node::Buffer::Data(GetFromPersistent(key)->ToObject());
        m_buf_len = info[1]->Uint32Value();
      }
    }
  ~FeedWorker()
    {
    }

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute () {
    if ((m_buf) && (m_buf_len)) {
      m_ret = m_obj->m_parser.feed(m_buf_len, (uint8_t*)m_buf);
    }
    if (m_ret < 0) {
      SetErrorMessage("invalid buffer / length");
    }
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback () {
    Nan::HandleScope scope;

    v8::Local<v8::Value> argv[] = {
        Nan::Null()
      , Nan::New<v8::Number>(m_ret)
    };

    callback->Call(2, argv);
  }

private:
  dvbteeParser* m_obj;
  char* m_buf;
  unsigned int m_buf_len;
  int m_ret;
};


void dvbteeParser::feed(const Nan::FunctionCallbackInfo<v8::Value>& info) {

  // If a callback function is supplied, this method will run async
  // otherwise, it will run synchronous
  //
  // Note: when packets are pushed to the parser, the parser will start
  // generating async events containing PSIP table data regardless of
  // whether this function was called synchronously or not

  int lastArg = info.Length() - 1;

  if ((lastArg >= 0) && (info[lastArg]->IsFunction())) {

    Nan::Callback *callback = new Nan::Callback(info[lastArg].As<v8::Function>());
    Nan::AsyncQueueWorker(new FeedWorker(callback, info));

  } else {
    dvbteeParser* obj = ObjectWrap::Unwrap<dvbteeParser>(info.Holder());
    int ret = -1;

    if ((info[0]->IsObject()) && (info[1]->IsNumber())) {

      v8::Local<v8::Object> bufferObj = info[0]->ToObject();
      unsigned int len = info[1]->Uint32Value();
      const char* buf = node::Buffer::Data(bufferObj);

      ret = obj->m_parser.feed(len, (uint8_t*)buf);
    }

    info.GetReturnValue().Set(Nan::New(ret));
  }
}
