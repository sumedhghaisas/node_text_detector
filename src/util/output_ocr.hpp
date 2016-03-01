#ifndef TEXTDETECTOR_OUTPUTOCR
#define TEXTDETECTOR_OUTPUTOCR

#include <vector>
#include <vector>
#include <node.h>
#include <nan.h>
#include <sstream>

#include "box.hpp"
#include "decoded_text.hpp"

struct OutputOCR
{
  OutputOCR(Box box, std::vector<DecodedText> text_vec)
    : box(box), text_vec(text_vec) {}
  Box box;
  std::vector<DecodedText> text_vec;  

  v8::Local<v8::Object> ToLocal()
  { 
    v8::Local<v8::Array> l_box = box.ToLocal();
    v8::Local<v8::Array> l_decodedArr = Nan::New<v8::Array>();
    for(int i=0;i<text_vec.size();i++)
    {
    	v8::Local<v8::Object> l_decodedtext = text_vec[i].ToLocal();
        Nan::Set(l_decodedArr, i, l_decodedtext);
    }
    
    v8::Local<v8::Object> out = Nan::New<v8::Object>();
    Nan::Set(out, Nan::New("box").ToLocalChecked(), l_box);
    Nan::Set(out, Nan::New("decodedText").ToLocalChecked(),l_decodedArr);
    return out; 
  }
};

#endif
