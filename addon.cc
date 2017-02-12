#include <nan.h>
#include "addon.h"
#include "dvbtee-parser.h"


void logLevel(const Nan::FunctionCallbackInfo<v8::Value>& info) {

  libdvbtee_set_debug_level((info[0]->IsNumber()) ? info[0]->Uint32Value() : 255,
                            (info[1]->IsNumber()) ? info[1]->Uint32Value() : 0);

  info.GetReturnValue().Set(info.Holder());
}

NAN_MODULE_INIT(InitAll) {

  Nan::Set(target, Nan::New<v8::String>("logLevel").ToLocalChecked(),
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(logLevel)).ToLocalChecked());

  dvbteeParser::Init(target);
}

NODE_MODULE(dvbtee, InitAll)
