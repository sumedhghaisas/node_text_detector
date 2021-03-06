#ifndef TEXTDETECTOR_BOX_HPP
#define TEXTDETECTOR_BOX_HPP

#include <node.h>

struct Box
{
  v8::Local<v8::Array> ToLocal()
  {
    v8::Local<v8::Array> array = Nan::New<v8::Array>();
    Nan::Set(array, 0, Nan::New<v8::Integer>(x1));
	Nan::Set(array, 1, Nan::New<v8::Integer>(x2));
	Nan::Set(array, 2, Nan::New<v8::Integer>(y1));
	Nan::Set(array, 3, Nan::New<v8::Integer>(y2));
	return array;
  }
  Box() {
  	x1=x2=y1=y2=NULL;
  }
  Box(int b[]){
    x1=b[0];
    x2=b[1];
    y1=b[2];
    y2=b[3];
  }
  Box(int x1, int x2, int y1, int y2)
    : x1(x1), x2(x2), y1(y1), y2(y2) {}

  int x1, x2, y1, y2;
};

#endif
