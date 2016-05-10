#include <iostream>
#include <node.h>
#include <nan.h>
//#include <sync.h>
#include "Begin.hpp"

using v8::FunctionTemplate;
using v8::Function;
using v8::Local;
using v8::Number;
using v8::String;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using Nan::To;
using Nan::GetFunction;
using Nan::Set;
using Nan::Utf8String;

NAN_METHOD(beginNan) {
    begin();
}

NAN_MODULE_INIT(Init)
{
    Set(target, Nan::New<v8::String>("beginSync").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(beginNan)).ToLocalChecked());
}

NODE_MODULE(new_ocr, Init)

