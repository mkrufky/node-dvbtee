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
#include "dvbtee-parser.h" // NOLINT(build/include)

void libVersion(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  v8::Local<v8::Array> version = Nan::New<v8::Array>();

  Nan::Set(version, 0, Nan::New<v8::Number>(LIBDVBTEE_VERSION_A));
  Nan::Set(version, 1, Nan::New<v8::Number>(LIBDVBTEE_VERSION_B));
  Nan::Set(version, 2, Nan::New<v8::Number>(LIBDVBTEE_VERSION_C));

  info.GetReturnValue().Set(version);
}

void logLevel(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  libdvbtee_set_debug_level(
      (info[0]->IsNumber()) ? Nan::To<uint32_t>(info[0]).FromJust() : 255,
      (info[1]->IsNumber()) ? Nan::To<uint32_t>(info[1]).FromJust() : 0);

  info.GetReturnValue().Set(info.Holder());
}

template <typename T>
v8::Local<v8::Array> vectorToV8Array(std::vector<T> v)
{
  v8::Local<v8::Array> a = Nan::New<v8::Array>();
  unsigned int pos = 0;

  for (typename std::vector<T>::const_iterator it = v.begin();
       it != v.end(); ++it)

    Nan::Set(a, pos++, Nan::New(*it));

  return a;
}

void getTableDecoderIds(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  info.GetReturnValue().Set(
      vectorToV8Array(dvbtee::decode::TableRegistry::instance().list()));
}

void getDescriptorDecoderIds(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
  info.GetReturnValue().Set(
      vectorToV8Array(dvbtee::decode::DescriptorRegistry::instance().list()));
}

NAN_MODULE_INIT(InitAll)
{
  Nan::Set(target,
           Nan::New<v8::String>("logLevel").ToLocalChecked(),
           Nan::GetFunction(
               Nan::New<v8::FunctionTemplate>(logLevel))
               .ToLocalChecked());

  Nan::Set(target,
           Nan::New<v8::String>("libVersion").ToLocalChecked(),
           Nan::GetFunction(
               Nan::New<v8::FunctionTemplate>(libVersion))
               .ToLocalChecked());

  Nan::Set(target,
           Nan::New<v8::String>("getTableDecoderIds").ToLocalChecked(),
           Nan::GetFunction(
               Nan::New<v8::FunctionTemplate>(getTableDecoderIds))
               .ToLocalChecked());

  Nan::Set(target,
           Nan::New<v8::String>("getDescriptorDecoderIds").ToLocalChecked(),
           Nan::GetFunction(
               Nan::New<v8::FunctionTemplate>(getDescriptorDecoderIds))
               .ToLocalChecked());

  dvbteeParser::Init(target);
}

NODE_MODULE(dvbtee, InitAll)
