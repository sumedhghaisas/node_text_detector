#ifndef TEXTDETECTOR_DECODEDTEXT
#define TEXTDETECTOR_DECODEDTEXT

#include <node.h>
#include <vector>
#include <string>
#include <nan.h>

#include "box.hpp"

struct DecodedText
{
    DecodedText(Box box, std::string word, float conf)
        : box(box), word(word), conf(conf) {}
    
    Box box;
    std::string word;
    float conf;
    v8::Local<v8::Object> object;

    v8::Local<v8::Object> ToLocal()
    {
        v8::Local<v8::Array> l_box = box.ToLocal();
        
        v8::Local<v8::Object> out = Nan::New<v8::Object>();
        Nan::Set(out, Nan::New("box").ToLocalChecked(), l_box);
        Nan::Set(out, Nan::New("value").ToLocalChecked(), Nan::New(word.c_str()).ToLocalChecked());
        Nan::Set(out, Nan::New("confidence").ToLocalChecked(), Nan::New(conf));
        return out;
    }
};
#endif
