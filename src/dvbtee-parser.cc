/*********************************************************************
 * node-dvbtee for Node.js
 *
 * Copyright (c) 2017 Michael Ira Krufky <https://github.com/mkrufky>
 *
 * MIT License <https://github.com/mkrufky/node-dvbtee/blob/master/LICENSE>
 *
 * See https://github.com/mkrufky/node-dvbtee for more information
 ********************************************************************/

#include <nan.h>
#include "dvbtee-parser.h"  // NOLINT(build/include)

TableData::TableData(const uint8_t &tableId,
                     const std::string &decoderName,
                     const std::string &json)
: tableId(tableId)
, decoderName(decoderName)
, json(json)
{}

TableData::TableData()
  : tableId(0)
{}

TableData::TableData(const TableData &t)
  : tableId(t.tableId)
  , decoderName(t.decoderName)
  , json(t.json)
{}

TableData &TableData::operator=(const TableData &cSource) {
  if (this == &cSource)
      return *this;

  tableId     = cSource.tableId;
  decoderName = cSource.decoderName;
  json        = cSource.json;

  return *this;
}

////////////////////////////////////////

Nan::Persistent<v8::Function> dvbteeParser::constructor;

dvbteeParser::dvbteeParser() {
}

dvbteeParser::~dvbteeParser() {
}

void dvbteeParser::Init(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE exports) {
  Nan::HandleScope scope;

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Parser").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "reset",        reset);
  Nan::SetPrototypeMethod(tpl, "feed",         feed);
  Nan::SetPrototypeMethod(tpl, "enableEttCollection", enableEttCollection);

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

////////////////////////////////////////

void dvbteeParser::enableEttCollection(
  const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dvbteeParser* obj = ObjectWrap::Unwrap<dvbteeParser>(info.Holder());

  obj->m_parser.enable_ett_collection();

  info.GetReturnValue().Set(info.Holder());
}

////////////////////////////////////////

class ResetWorker : public Nan::AsyncWorker {
 public:
  ResetWorker(Nan::Callback *callback,
              const Nan::FunctionCallbackInfo<v8::Value>& info)
    : Nan::AsyncWorker(callback) {
      m_obj = Nan::ObjectWrap::Unwrap<dvbteeParser>(info.Holder());
    }
  ~ResetWorker() {
    }

  void Execute () {
    m_obj->m_parser.cleanup();
    m_obj->m_parser.reset();
  }

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
    Nan::Callback *callback = new Nan::Callback(
      info[lastArg].As<v8::Function>()
    );
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
  FeedWorker(Nan::Callback *callback,
             const Nan::FunctionCallbackInfo<v8::Value>& info)
    : Nan::AsyncWorker(callback)
    , m_buf(NULL)
    , m_buf_len(0)
    , m_ret(-1) {
      m_obj = Nan::ObjectWrap::Unwrap<dvbteeParser>(info.Holder());

      if ((info[0]->IsObject()) && (info[1]->IsNumber())) {
        const char *key = "buf";
        SaveToPersistent(key, info[0]);
        Nan::MaybeLocal<v8::Object> mbBuf =
          Nan::To<v8::Object>(GetFromPersistent(key));

        if (!mbBuf.IsEmpty()) {
          m_buf = node::Buffer::Data(mbBuf.ToLocalChecked());
          m_buf_len = info[1]->Uint32Value();
        }
      }
    }
  ~FeedWorker() {
    }

  void Execute () {
    if ((m_buf) && (m_buf_len)) {
      m_ret = m_obj->m_parser.feed(
        m_buf_len, reinterpret_cast<uint8_t*>(m_buf)
      );
    }
    if (m_ret < 0) {
      SetErrorMessage("invalid buffer / length");
    }
  }

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

////////////////////////////////////////

class FeedProgressWorker:
public dvbtee::decode::TableWatcher,
public Nan::AsyncProgressQueueWorker<TableData> {
 public:
  FeedProgressWorker(Nan::Callback *callback,
             Nan::Callback *progress,
             const Nan::FunctionCallbackInfo<v8::Value>& info)
    : Nan::AsyncProgressQueueWorker<TableData>(callback)
    , m_cb_progress(progress)
    , m_buf(NULL)
    , m_buf_len(0)
    , m_ret(-1) {
      m_obj = Nan::ObjectWrap::Unwrap<dvbteeParser>(info.Holder());

      if ((info[0]->IsObject()) && (info[1]->IsNumber())) {
        const char *key = "buf";
        SaveToPersistent(key, info[0]);
        Nan::MaybeLocal<v8::Object> mbBuf =
          Nan::To<v8::Object>(GetFromPersistent(key));

        if (!mbBuf.IsEmpty()) {
          m_buf = node::Buffer::Data(mbBuf.ToLocalChecked());
          m_buf_len = info[1]->Uint32Value();
        }
      }
    }
  ~FeedProgressWorker() {
    }

  void Execute (const AsyncProgressQueueWorker::ExecutionProgress& progress) {
    if ((m_buf) && (m_buf_len)) {
      m_progress = &progress;                 // XXX
      m_obj->m_parser.subscribeTables(this);  // XXX

      m_ret = m_obj->m_parser.feed(
        m_buf_len, reinterpret_cast<uint8_t*>(m_buf)
      );
    }
    if (m_ret < 0) {
      SetErrorMessage("invalid buffer / length");
    }
  }

  void updateTable(uint8_t tId, dvbtee::decode::Table *table) {
    m_progress->Send(
      new TableData(table->getTableid(),
      table->getDecoderName(),
      table->toJson()), 1
    );
  }

  void HandleProgressCallback(const TableData *async_data, size_t count) {
    Nan::HandleScope scope;

    if (!m_cb_progress->IsEmpty()) {
      for (unsigned int i = 0; i < count; i++) {
        const TableData *data = &async_data[i];

        Nan::MaybeLocal<v8::String> jsonStr = Nan::New(data->json);
        if (!jsonStr.IsEmpty()) {
          Nan::MaybeLocal<v8::Value> jsonVal =
            NanJSON.Parse(jsonStr.ToLocalChecked());

          if (!jsonVal.IsEmpty()) {
            Nan::MaybeLocal<v8::String> decoderName =
              Nan::New(data->decoderName);

            v8::Local<v8::Value> decoderNameArg;
            if (decoderName.IsEmpty())
              decoderNameArg = Nan::Null();
            else
              decoderNameArg = decoderName.ToLocalChecked();

            v8::Local<v8::Value> argv[] = {
              Nan::New(data->tableId),
              decoderNameArg,
              jsonVal.ToLocalChecked()
            };

            m_cb_progress->Call(3, argv);
          }
        }
      }
    }
  }

  void HandleOKCallback () {
    Nan::HandleScope scope;

    v8::Local<v8::Value> argv[] = {
        Nan::Null()
      , Nan::New<v8::Number>(m_ret)
    };

    callback->Call(2, argv);
  }

 private:
  Nan::Callback *m_cb_progress;
  Nan::JSON NanJSON;
  const AsyncProgressQueueWorker::ExecutionProgress *m_progress;
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

  dvbteeParser* obj = ObjectWrap::Unwrap<dvbteeParser>(info.Holder());

  int lastArg = info.Length() - 1;

  if ((lastArg >= 1) && (info[lastArg]->IsFunction()) &&
      (info[lastArg - 1]->IsFunction())) {
    Nan::Callback *progress = new Nan::Callback(
      info[lastArg - 1].As<v8::Function>()
    );
    Nan::Callback *callback = new Nan::Callback(
      info[lastArg].As<v8::Function>()
    );
    Nan::AsyncQueueWorker(new FeedProgressWorker(callback, progress, info));

  } else if ((lastArg >= 0) && (info[lastArg]->IsFunction())) {
    Nan::Callback *callback = new Nan::Callback(
      info[lastArg].As<v8::Function>()
    );
    Nan::AsyncQueueWorker(new FeedWorker(callback, info));

  } else {
    int ret = -1;

    if ((info[0]->IsObject()) && (info[1]->IsNumber())) {
      Nan::MaybeLocal<v8::Object> bufferObj = Nan::To<v8::Object>(info[0]);

      if (!bufferObj.IsEmpty()) {
        unsigned int len = info[1]->Uint32Value();
        char* buf = node::Buffer::Data(bufferObj.ToLocalChecked());

        ret = obj->m_parser.feed(len, reinterpret_cast<uint8_t*>(buf));
      }
    }

    info.GetReturnValue().Set(Nan::New(ret));
  }
}
