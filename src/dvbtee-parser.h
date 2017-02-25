#ifndef _DVBTEE_PARSER_H_
#define _DVBTEE_PARSER_H_

#include <nan.h>
#include <queue>
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

class TableReceiver: public dvbtee::decode::TableWatcher
{
public:
  TableReceiver(dvbteeParser *dvbteeParser);
  virtual ~TableReceiver();

  void subscribe(const v8::Local<v8::Function> &fn);

private:
  static uv_async_t m_async;
  uv_mutex_t m_ev_mutex;
  dvbteeParser *m_dvbteeParser;
  std::queue<TableData*> m_eq;
  Nan::Callback m_cb;

  void updateTable(uint8_t tId, dvbtee::decode::Table *table);
  void notify();
  void registerInterface();
  void unregisterInterface();

  static void completeCb(uv_async_t *handle
#if NODE_MODULE_VERSION<=11
                                           , int status /*UNUSED*/
#endif
                        );
};


class dvbteeParser : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  explicit dvbteeParser();
  ~dvbteeParser();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void reset(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void feed(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void listenTables(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
  TableReceiver m_tableReceiver;
  privateParse m_parser;

  friend class FeedWorker;
  friend class ResetWorker;
  friend class TableReceiver;
};

#endif  // _DVBTEE_PARSER_H_
