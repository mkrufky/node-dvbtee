#include <nan.h>
#include "addon.h"
#include "dvbtee-parser.h"

TableData::TableData(const uint8_t &tableId, const std::string &decoderName, const std::string &json)
: tableId(tableId)
, decoderName(decoderName)
, json(json)
{}

////////////////////////////////////////

uv_async_t TableReceiver::m_async;

TableReceiver::TableReceiver(dvbteeParser *dvbteeParser)
: m_dvbteeParser(dvbteeParser)
{
  uv_mutex_init(&m_mutex);
  uv_async_init(uv_default_loop(), &m_async, completeCb);
  m_async.data = this;
}

TableReceiver::~TableReceiver()
{
  uv_mutex_destroy(&m_mutex);
}

void TableReceiver::updateTable(uint8_t tId, dvbtee::decode::Table *table)
{
  uv_mutex_lock(&m_mutex);
  v.push_back(new TableData(table->getTableid(), table->getDecoderName(), table->toJson()));
  uv_mutex_unlock(&m_mutex);

  uv_async_send(&m_async);
}

void TableReceiver::notify()
{
  Nan::HandleScope scope;
  uv_mutex_lock(&m_mutex);
  for (std::vector<TableData*>::const_iterator it = v.begin(); it != v.end(); ++it)
  {
    TableData *data = *it;
    v8::Local<v8::String> jsonStr = Nan::New(data->json).ToLocalChecked();
    
    v8::Local<v8::Value> argv[] = {
      Nan::New(data->tableId),
      Nan::New(data->decoderName).ToLocalChecked(),
      v8::JSON::Parse(jsonStr)
    };
    m_dvbteeParser->m_cb_tableReceiver.Call(3, argv);

    delete data;
  }
  v.clear();
  uv_mutex_unlock(&m_mutex);
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

dvbteeParser::dvbteeParser(double value)
: m_tableReceiver(this)
, value_(value) {
}

dvbteeParser::~dvbteeParser() {
}

void dvbteeParser::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Parser").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "reset", reset);
  Nan::SetPrototypeMethod(tpl, "push", push);
  Nan::SetPrototypeMethod(tpl, "listenTables", listenTables);
  Nan::SetPrototypeMethod(tpl, "logLevel", logLevel);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Parser").ToLocalChecked(), tpl->GetFunction());
}

void dvbteeParser::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new dvbteeParser(...)`
    double value = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
    dvbteeParser* obj = new dvbteeParser(value);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    // Invoked as plain function `dvbteeParser(...)`, turn into construct call.
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { info[0] };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void dvbteeParser::listenTables(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dvbteeParser* obj = ObjectWrap::Unwrap<dvbteeParser>(info.Holder());

  int lastArg = info.Length() - 1;

  if ((lastArg >= 0) && (info[lastArg]->IsFunction())) {
    obj->m_cb_tableReceiver.SetFunction(info[lastArg].As<v8::Function>());
    obj->m_parser.subscribeTables(&obj->m_tableReceiver);
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

  int lastArg = info.Length() - 1;

  if ((lastArg >= 0) && (info[lastArg]->IsFunction())) {

    Nan::Callback *callback = new Nan::Callback(info[lastArg].As<v8::Function>());
    Nan::AsyncQueueWorker(new ResetWorker(callback, info));

  } else {
    dvbteeParser* obj = ObjectWrap::Unwrap<dvbteeParser>(info.Holder());

    obj->m_parser.cleanup();
    obj->m_parser.reset();

    info.GetReturnValue().Set(Nan::New(0));
  }
}

////////////////////////////////////////

class PushWorker : public Nan::AsyncWorker {
public:
  PushWorker(Nan::Callback *callback, const Nan::FunctionCallbackInfo<v8::Value>& info)
    : Nan::AsyncWorker(callback)
    {
      m_obj = Nan::ObjectWrap::Unwrap<dvbteeParser>(info.Holder());

      if ((info[0]->IsObject()) && (info[1]->IsNumber())) {

        v8::Local<v8::Object> bufferObj = info[0]->ToObject();
        m_buf_len = info[1]->Uint32Value();
        m_buf = (char*) malloc(m_buf_len);

        memcpy(m_buf, node::Buffer::Data(bufferObj), m_buf_len);
      }
    }
  ~PushWorker()
    {
      if (m_buf) free(m_buf);
    }

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute () {
    m_ret = m_obj->m_parser.feed(m_buf_len, (uint8_t*)m_buf);
    free(m_buf);
    m_buf = NULL;
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


void dvbteeParser::push(const Nan::FunctionCallbackInfo<v8::Value>& info) {

  int lastArg = info.Length() - 1;

  if ((lastArg >= 0) && (info[lastArg]->IsFunction())) {

    Nan::Callback *callback = new Nan::Callback(info[lastArg].As<v8::Function>());
    Nan::AsyncQueueWorker(new PushWorker(callback, info));

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
