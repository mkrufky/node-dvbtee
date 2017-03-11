#ifndef __NATIVE_JSON_H__
#define __NATIVE_JSON_H__

#include <nan.h>

namespace Native {

class JSON {
public:
  static v8::Local<v8::Value> Parse(v8::Local<v8::Value> jsonString)
  {
    return JSON::instance().parse(jsonString);
  }

private:
  Nan::Callback m_cb_parse;

  static JSON& instance()
  {
    static JSON i;
    return i;
  }

  JSON()
  {
    v8::Local<v8::Value> globalJSON = Nan::GetCurrentContext()->Global()->Get(Nan::New("JSON").ToLocalChecked());

    if (globalJSON->IsObject()) {
      v8::Local<v8::Value> parseMethod = globalJSON->ToObject()->Get(Nan::New("parse").ToLocalChecked());

      if (!parseMethod.IsEmpty() && parseMethod->IsFunction()) {
        m_cb_parse.Reset(v8::Local<v8::Function>::Cast(parseMethod));
      }
    }
  }

  ~JSON()
  {
    m_cb_parse.Reset();
  }

  v8::Local<v8::Value> parse(v8::Local<v8::Value> arg)
  {
    return m_cb_parse.Call(1, &arg);
  }
#if __cplusplus <= 199711L
private:
  JSON(JSON const&);
  void operator=(JSON const&);
#else
public:
  JSON(JSON const&)           = delete;
  void operator=(JSON const&) = delete;
#endif
};

}

#endif /* __NATIVE_JSON_H__ */
