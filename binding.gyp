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
      "sources": [ "src/addons/hse/hse_napi.cpp" ],
      "include_dirs": ["src/addons/hse/include","src/addons/hse/include/hse_header","src/addons/hse/include/hse_header/hse_common",
                      "src/addons/hse/include/hse_header/hse_config","src/addons/hse/include/hse_header/hse_services","<!@(node -p \"require('node-addon-api').include\")"],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS','NAPI_EXPERIMENTAL'],
      'libraries': ['<(module_root_dir)/public/mbedtls/mbedcrypto.lib','<(module_root_dir)/public/mbedtls/mbedtls.lib','<(module_root_dir)/public/mbedtls/mbedx509.lib','msvcrtd.lib']
    }
  ]
}