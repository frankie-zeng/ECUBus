{
  "targets": [
    {
      "target_name": "PCANTP",
      "sources": [ "src/addons/can/peak/pcantp_napi.cpp" ],
      "include_dirs": ["src/addons/","<!@(node -p \"require('node-addon-api').include\")"],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS','NAPI_EXPERIMENTAL' ]
    },
    {
      "target_name": "PLIN",
      "sources": [ "src/addons/can/peak/plin_napi.cpp" ],
      "include_dirs": ["src/addons/","<!@(node -p \"require('node-addon-api').include\")"],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS','NAPI_EXPERIMENTAL']
    },
    {
      "target_name": "HSE",
      "sources": [ "src/hse/hse/hse_napi.cpp" ],
      "include_dirs": ["src/hse/hse/include","src/hse/hse/include/hse_header","src/hse/hse/include/hse_header/hse_common",
                      "src/hse/hse/include/hse_header/hse_config","src/hse/hse/include/hse_header/hse_services","<!@(node -p \"require('node-addon-api').include\")"],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS','NAPI_EXPERIMENTAL'],
      'libraries': ['<(module_root_dir)/src/hse/hse/libcrypto_static.lib']
    }
  ]
}