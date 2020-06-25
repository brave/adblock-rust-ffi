{
  "targets": [{
    "target_name": "adblock-rust",
    "sources": [
      "src/addon.cc",
      "src/ad_block_client_wrap.cc",
      "src/ad_block_client_wrap.h",
      "src/lib.h",
      "src/lib.rs",
      "src/wrapper.cpp",
      "src/wrapper.hpp"
    ],
    "libraries": [ "../target/debug/libadblock.a" ],
    "include_dirs": [
      "src/"
    ],
    "dependencies": [
    ],
    "conditions": [
      ['OS=="win"', {
        }, {
          'cflags_cc': [ '-fexceptions' ]
        }
      ]
    ],
    "xcode_settings": {
      "OTHER_CFLAGS": [ "-ObjC" ],
      "OTHER_CPLUSPLUSFLAGS" : ["-std=c++11","-stdlib=libc++", "-v"],
      "MACOSX_DEPLOYMENT_TARGET": "10.9",
      "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
    },
  }]
}
