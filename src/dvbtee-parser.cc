/*********************************************************************
 * node-dvbtee for Node.js
 *
 * Copyright (c) 2018 Michael Ira Krufky <https://github.com/mkrufky>
 *
 * MIT License <https://github.com/mkrufky/node-dvbtee/blob/master/LICENSE>
 *
 * See https://github.com/mkrufky/node-dvbtee for more information
 ********************************************************************/

#include <nan.h>
#include "dvbtee-parser.h"               // NOLINT(build/include)
#if NAN_HAS_CPLUSPLUS_11
#include <async_factory_worker.h>        // NOLINT(build/include)
#else
#include "async_factory_worker_cpp03.h"  // NOLINT(build/include)
#endif

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

  Nan::SetPrototypeMethod(tpl, "reset"              , reset);
  Nan::SetPrototypeMethod(tpl, "feed"               , feed);
  Nan::SetPrototypeMethod(tpl, "enableEttCollection", enableEttCollection);
  Nan::SetPrototypeMethod(tpl, "enableParseISO6937" , enableParseISO6937);

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
    info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
  }
}

////////////////////////////////////////

void dvbteeParser::enableEttCollection(
  const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dvbteeParser* obj = ObjectWrap::Unwrap<dvbteeParser>(info.Holder());

  bool enable = Nan::To<bool>(info[0]).FromMaybe(true);

  obj->m_parser.enable_ett_collection(enable);

  info.GetReturnValue().Set(info.Holder());
}

////////////////////////////////////////

void dvbteeParser::enableParseISO6937(
  const Nan::FunctionCallbackInfo<v8::Value>& info) {
  dvbteeParser* obj = ObjectWrap::Unwrap<dvbteeParser>(info.Holder());

  bool enable = Nan::To<bool>(info[0]).FromMaybe(true);

  obj->m_parser.enable_iso6937_translation(enable);

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

class TableData {
 public:
  explicit TableData(
  const uint8_t& tableId,
  const std::string& decoderName,
  const std::string& json)
  : tableId(tableId)
  , decoderName(decoderName)
  , json(json) {
  }

  const uint8_t tableId;
  const std::string decoderName;
  const std::string json;

 private:
  NAN_DISALLOW_ASSIGN_COPY(TableData)
#if NAN_HAS_CPLUSPLUS_11
  TableData() = delete;
#else
  TableData();
#endif  // NAN_HAS_CPLUSPLUS_11
};

class FeedWorker: public Nan::Mkrufky::AsyncFactoryWorker<TableData,
  const uint8_t&, const std::string&, const std::string&> {
 public:
  FeedWorker(Nan::Callback *callback,
             Nan::Callback *progress,
             const Nan::FunctionCallbackInfo<v8::Value>& info,
             const char* resource_name = "dvbtee:FeedWorker")
    : AsyncFactoryWorker(callback, resource_name)
    , m_progress(progress)
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
    delete m_progress;
  }

  void Execute(const AsyncFactoryWorker::ExecutionProgress& progress) {
    if ((m_buf) && (m_buf_len)) {
      Watcher watch(this->m_obj, progress);

      m_ret = m_obj->m_parser.feed(
        m_buf_len, reinterpret_cast<uint8_t*>(m_buf)
      );
    }
    if (m_ret < 0) {
      SetErrorMessage("invalid buffer / length");
    }
  }

  void HandleProgressCallback(const TableData *async_data, size_t count) {
    Nan::HandleScope scope;

    if (!m_progress->IsEmpty()) {
      for (unsigned int i = 0; i < count; i++) {
        const TableData *data = &async_data[i];

        Nan::MaybeLocal<v8::String> jsonStr = Nan::New(data->json);
        if (!jsonStr.IsEmpty()) {
          Nan::MaybeLocal<v8::Value> jsonVal =
            m_obj->NanJSON.Parse(jsonStr.ToLocalChecked());

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

            m_progress->Call(3, argv);
          }
        }
      }
    }
  }

  void HandleOKCallback() {
    Nan::HandleScope scope;

    v8::Local<v8::Value> argv[] = {
        Nan::Null()
      , Nan::New<v8::Number>(m_ret)
    };

    callback->Call(2, argv);
  }

 private:
  Nan::Callback *m_progress;
  dvbteeParser* m_obj;
  char* m_buf;
  unsigned int m_buf_len;
  int m_ret;

  class Watcher: public dvbtee::decode::TableWatcher {
   public:
    explicit Watcher(dvbteeParser* obj,
      const AsyncFactoryWorker::ExecutionProgress& progress)
     : m_obj(obj)
     , m_progress(progress) {
      m_obj->m_parser.subscribeTables(this);
    }

    ~Watcher() {
      m_obj->m_parser.subscribeTables(NULL);
    }

    void updateTable(uint8_t tId, dvbtee::decode::Table *table) {
      m_progress.Construct(
        table->getTableid(),
        table->getDecoderName(),
        table->toJson()
      );
    }

   private:
    dvbteeParser* m_obj;
    const AsyncFactoryWorker::ExecutionProgress& m_progress;
  };
};

void dvbteeParser::feed(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  int lastArg = info.Length() - 1;

  if ((lastArg >= 1) && (info[lastArg]->IsFunction()) &&
      (info[lastArg - 1]->IsFunction())) {
    Nan::Callback *progress = new Nan::Callback(
      info[lastArg - 1].As<v8::Function>()
    );
    Nan::Callback *callback = new Nan::Callback(
      info[lastArg].As<v8::Function>()
    );
    Nan::AsyncQueueWorker(new FeedWorker(callback, progress, info));

  } else {
    info.GetReturnValue().Set(Nan::New(-1));
  }
}
