/*********************************************************************
 * node-dvbtee for Node.js
 *
 * Copyright (c) 2017 Michael Ira Krufky <https://github.com/mkrufky>
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
#include "parse.h"

class TableData {
 public:
  TableData(const uint8_t&, const std::string&, const std::string&);

  const uint8_t tableId;
  const std::string decoderName;
  const std::string json;
};

class dvbteeParser;

class TableReceiver: public dvbtee::decode::TableWatcher {
 public:
  explicit TableReceiver(dvbteeParser *dvbteeParser);
  virtual ~TableReceiver();

  void subscribe(const v8::Local<v8::Function> &fn);

 private:
  uv_async_t m_async;
  uv_mutex_t m_ev_mutex;
  dvbteeParser *m_dvbteeParser;
  std::queue<TableData*> m_eq;
  Nan::Callback m_cb;
  Nan::JSON NanJSON;

  void updateTable(uint8_t tId, dvbtee::decode::Table *table);
  void notify();
  void registerInterface();
  void unregisterInterface();

  static NAUV_WORK_CB(completeCb);
};


class dvbteeParser : public Nan::ObjectWrap {
 public:
  static void Init(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE exports);

 private:
  dvbteeParser();
  virtual ~dvbteeParser();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void reset(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void feed(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void listenTables(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void enableEttCollection(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
  TableReceiver m_tableReceiver;
  PrivateParse m_parser;

  friend class FeedWorker;
  friend class ResetWorker;
  friend class TableReceiver;
};

#endif  // SRC_DVBTEE_PARSER_H_
