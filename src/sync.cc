#include <iostream>
#include <node.h>
#include <nan.h>
#include "sync.h"
#include "ocr/ocr.hpp"

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

// Simple synchronous access to the `GetText()` function
NAN_METHOD(GetTextSync) {
    // get the value of path
    Utf8String *utfPath = new Utf8String(info[0]);
    Utf8String *utfLanguageFile = new Utf8String(info[1]);
    string path(**utfPath);
    string languageFile(**utfLanguageFile);
    // if the second argument is passed, we use it
    if (info.Length() > 1){
        //detectRegions = To<bool>(info[1]).FromJust();
    }
    // call the decoder here
    OutputOCR decodedText = Ocr(path, languageFile);
    // set the return value
    info.GetReturnValue().Set(Nan::New(decodedText.ToLocal()));
}

NAN_MODULE_INIT(Init)
{
   Set(target, Nan::New<v8::String>("GetTextSync").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(GetTextSync)).ToLocalChecked());
}

NODE_MODULE(node_text_detector, Init)
