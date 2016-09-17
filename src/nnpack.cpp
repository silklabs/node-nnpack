/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set shiftwidth=2 tabstop=2 autoindent cindent expandtab: */

/**
 * Copyright (c) 2015-2016 Silk Labs, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Silk Labs, Inc.
 */

#include <node.h>
#include <nan.h>

#include <nnpack.h>

using v8::Float32Array;
using v8::FunctionTemplate;
using v8::Handle;
using v8::Local;
using v8::Object;
using v8::Uint32;
using v8::Value;

using node::AtExit;

using Nan::FunctionCallbackInfo;
using Nan::Maybe;
using Nan::MaybeLocal;
using Nan::New;
using Nan::ThrowTypeError;
using Nan::To;
using Nan::TypedArrayContents;
using Nan::Undefined;

static pthreadpool_t threadpool;

// Get an argument
static Local<Value> Arg(const FunctionCallbackInfo<Value>& info, int n) {
  if (n < info.Length())
    return info[n];
  return Undefined();
}

static NAN_METHOD(Relu) {
  uint32_t batch_size = To<uint32_t>(Arg(info, 0)).FromMaybe(0);
  uint32_t channels = To<uint32_t>(Arg(info, 1)).FromMaybe(0);
  MaybeLocal<Object> input = To<Object>(Arg(info, 2));
  MaybeLocal<Object> output = To<Object>(Arg(info, 3));
  double negative_slope = To<double>(Arg(info, 4)).FromMaybe(0);
  if (input.IsEmpty())
    return ThrowTypeError("missing input array");
  if (output.IsEmpty())
    return ThrowTypeError("missing output array");
  TypedArrayContents<float> input_array(input.ToLocalChecked());
  TypedArrayContents<float> output_array(output.ToLocalChecked());
  if (input_array.length() < batch_size * channels)
    return ThrowTypeError("input array too short");
  if (output_array.length() < batch_size * channels)
    return ThrowTypeError("output array too short");
  info.GetReturnValue().Set(int32_t(nnp_relu_output(batch_size,
                                                    channels,
                                                    *input_array,
                                                    *output_array,
                                                    negative_slope,
                                                    threadpool)));
}

void Exit(void *) {
  nnp_deinitialize();
  if (threadpool)
    pthreadpool_destroy(threadpool);
}

void Init(Handle<Object> exports) {
  AtExit(Exit);
  // create a thread pool to be used by this module
  threadpool = pthreadpool_create(0);
  nnp_initialize();
  exports->Set(New("relu").ToLocalChecked(), New<FunctionTemplate>(Relu)->GetFunction());
}

NODE_MODULE(NNPACK, Init);
