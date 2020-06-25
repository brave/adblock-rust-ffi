/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef AD_BLOCK_CLIENT_WRAP_H_
#define AD_BLOCK_CLIENT_WRAP_H_

#include <node.h>
#include <node_object_wrap.h>

namespace ad_block_client_wrap {

class AdBlockClientWrap : public node::ObjectWrap {
  public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  AdBlockClientWrap();
  virtual ~AdBlockClientWrap();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void GenerateDefaultManifestFile(
      const v8::FunctionCallbackInfo<v8::Value>& args);
  static void GenerateRegionalManifestFiles(
      const v8::FunctionCallbackInfo<v8::Value>& args);
  static v8::Persistent<v8::Function> constructor;
};

}  // ad_block_client_wrap

#endif  // AD_BLOCK_CLIENT_WRAP_H_