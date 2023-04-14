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
    }
  ]
}