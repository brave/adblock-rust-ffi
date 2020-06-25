/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ad_block_client_wrap.h"

#include <node_buffer.h>
#include <algorithm>
#include <fstream>

#include "wrapper.hpp"

namespace ad_block_client_wrap {

using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Int32;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Boolean;
using v8::Value;
using v8::NewStringType;

#if V8_MAJOR_VERSION >= 7
#define CHECK_SET(X) X.Check()
#else
#define CHECK_SET(X) (void)X
#endif

Persistent<Function> AdBlockClientWrap::constructor;

const std::string kAdBlockDefaultComponentId("cffkpbalmllkdoenhmdmpbkajipdjfam");  // NOLINT
const std::string kAdBlockDefaultBase64PublicKey =
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAs0qzJmHSgIiw7IGFCxij"
    "1NnB5hJ5ZQ1LKW9htL4EBOaMJvmqaDs/wfq0nw/goBHWsqqkMBynRTu2Hxxirvdb"
    "cugn1Goys5QKPgAvKwDHJp9jlnADWm5xQvPQ4GE1mK1/I3ka9cEOCzPW6GI+wGLi"
    "VPx9VZrxHHsSBIJRaEB5Tyi5bj0CZ+kcfMnRTsXIBw3C6xJgCVKISQUkd8mawVvG"
    "vqOhBOogCdb9qza5eJ1Cgx8RWKucFfaWWxKLOelCiBMT1Hm1znAoVBHG/blhJJOD"
    "5HcH/heRrB4MvrE1J76WF3fvZ03aHVcnlLtQeiNNOZ7VbBDXdie8Nomf/QswbBGa"
    "VwIDAQAB";

void GenerateManifestFile(const std::string &name,
                          const std::string &base64_public_key,
                          const std::string &filename) {
  std::ofstream outFile(filename);
  if (outFile.is_open()) {
    outFile << "{" << std::endl;
    outFile << "  \"description\": \"Brave Ad Block Updater extension\","
      << std::endl;
    outFile << "  \"key\": \"" << base64_public_key << "\"," << std::endl;
    outFile << "  \"manifest_version\": 2," << std::endl;
    outFile << "  \"name\": \"Brave Ad Block Updater (" << name << ")\","
      << std::endl;
    outFile << "  \"version\": \"0.0.0\"" << std::endl;
    outFile << "}" << std::endl;
  }
}

void AdBlockClientWrap::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new AdBlockClient(...)`
    AdBlockClientWrap* obj = new AdBlockClientWrap();
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `AdBlockClient(...)`,
    // turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(
        cons->NewInstance(isolate->GetCurrentContext(), argc, argv).ToLocalChecked());
  }
}

AdBlockClientWrap::AdBlockClientWrap() {
}

AdBlockClientWrap::~AdBlockClientWrap() {
}

void AdBlockClientWrap::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "AdBlockClient", NewStringType::kNormal).ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "generateDefaultManifestFile",
    AdBlockClientWrap::GenerateDefaultManifestFile);
  NODE_SET_PROTOTYPE_METHOD(tpl, "generateRegionalManifestFiles",
    AdBlockClientWrap::GenerateRegionalManifestFiles);

  constructor.Reset(isolate,
      tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked());
  CHECK_SET(exports->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "AdBlockClient", NewStringType::kNormal).ToLocalChecked(),
      tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked()));
}

void AdBlockClientWrap::GenerateDefaultManifestFile(
    const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  String::Utf8Value str(isolate, args[0]->ToString(isolate->GetCurrentContext())
      .FromMaybe(Local<String>()));
  const char * dir = *str;
  std::string filename = dir + std::string("/default-manifest.json");
  GenerateManifestFile("Default", kAdBlockDefaultBase64PublicKey, filename);
}

void AdBlockClientWrap::GenerateRegionalManifestFiles(
    const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  String::Utf8Value str(isolate, args[0]->ToString(isolate->GetCurrentContext())
      .FromMaybe(Local<String>()));
  const char * dir = *str;
  for (auto& entry : adblock::FilterList::GetRegionalLists()) {
    const std::string filename = (
      dir +
      std::string("/") +
      entry.uuid +
      std::string("-manifest.json"));
    GenerateManifestFile(entry.title, entry.base64_public_key, filename);
  }
}

}  // namespace ad_block_client_wrap
