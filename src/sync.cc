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
    OutputOCR* decodedText = NULL;
    if(!node::Buffer::HasInstance(info[0]))
    {
        // get the value of path
        Utf8String *utfPath = new Utf8String(info[0]);
        Utf8String *utfLanguageFile = new Utf8String(info[1]);
        Utf8String *utfWhitelist = new Utf8String(info[2]);
        string path(**utfPath);
        string languageFile(**utfLanguageFile);
        string whitelist(**utfWhitelist);
        
        decodedText = Ocr(path, languageFile, whitelist);
    }
    else
    {
        char* image = (char*)node::Buffer::Data(info[0]->ToObject());
        unsigned int len = node::Buffer::Length(info[0]->ToObject());
        Utf8String *utfLanguageFile = new Utf8String(info[1]);
        Utf8String *utfWhitelist = new Utf8String(info[2]);
        string languageFile(**utfLanguageFile);
        string whitelist(**utfWhitelist);
        
        decodedText = Ocr(image, len, languageFile, whitelist);
    }
   
    if(decodedText == NULL)
        info.GetReturnValue().Set(Nan::Undefined());
    else info.GetReturnValue().Set(Nan::New(decodedText->ToLocal()));
    
    delete decodedText;
}

NAN_MODULE_INIT(Init)
{
   Set(target, Nan::New<v8::String>("GetTextSync").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(GetTextSync)).ToLocalChecked());
}

NODE_MODULE(scene_text_detector, Init)
