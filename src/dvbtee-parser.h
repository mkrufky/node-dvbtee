/*********************************************************************
 * node-dvbtee for Node.js
 *
 * Copyright (c) 2018 Michael Ira Krufky <https://github.com/mkrufky>
 *
 * MIT License <https://github.com/mkrufky/node-dvbtee/blob/master/LICENSE>
 *
 * See https://github.com/mkrufky/node-dvbtee for more information
 ********************************************************************/

#ifndef SRC_DVBTEE_PARSER_H_
#define SRC_DVBTEE_PARSER_H_

#include <nan.h>
#include <queue>
#include <string>
#include <vector>
#include "parse.h" // NOLINT(build/include)

class dvbteeParser : public Nan::ObjectWrap
{
public:
  static void Init(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE exports);

private:
  dvbteeParser();
  virtual ~dvbteeParser();

  static void New(const Nan::FunctionCallbackInfo<v8::Value> &info);

  static void reset(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void feed(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void enableEttCollection(const Nan::FunctionCallbackInfo<v8::Value> &info);
  static void enableParseISO6937(const Nan::FunctionCallbackInfo<v8::Value> &info);

  static Nan::Persistent<v8::Function> constructor;
  Nan::JSON NanJSON;
  PrivateParse m_parser;

  friend class FeedWorker;
  friend class ResetWorker;
};

#endif // SRC_DVBTEE_PARSER_H_
