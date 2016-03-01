#ifndef TEXTDETECTOR_DECODEDTEXT
#define TEXTDETECTOR_DECODEDTEXT

#include <node.h>
#include <vector>
#include <string>
#include <nan.h>

#include "box.hpp"

struct DecodedText
{
    DecodedText(Box box, std::vector<std::string> words, std::vector<float> confs)
        : box(box), words(words), confs(confs) {}
    
    Box box;
    std::vector<std::string> words;
    std::vector<float> confs;
    v8::Local<v8::Object> object;

    v8::Local<v8::Object> ToLocal()
    {
        v8::Local<v8::Array> l_box = box.ToLocal();
        v8::Local<v8::Array> l_words = Nan::New<v8::Array>();
        v8::Local<v8::Array> l_confs = Nan::New<v8::Array>();

        for(int i = 0;i < words.size();i++)
        {
            Nan::Set(l_words, i, Nan::New(words[i]).ToLocalChecked());
        }

        for(int i=0;i<confs.size();i++)
        {
            Nan::Set(l_confs, i, Nan::New(confs[i]));
        }
        
        v8::Local<v8::Object> out = Nan::New<v8::Object>();
        Nan::Set(out, Nan::New("box").ToLocalChecked(), l_box);
        Nan::Set(out, Nan::New("words").ToLocalChecked(), l_words);
        Nan::Set(out, Nan::New("confidences").ToLocalChecked(), l_confs);
        return out;
    }
};
#endif
