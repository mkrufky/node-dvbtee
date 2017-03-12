{
  "targets": [
    {
      "target_name": "dvbtee",
      "sources": [
        "src/node-dvbtee.cc",
        "src/dvbtee-parser.cc"
      ],
      "dependencies": [
        'deps/libdvbtee.gyp:dvbtee_parser'
      ],
      "include_dirs": [
        "libdvbtee/usr/include",
        "libdvbtee/libdvbtee",
        "libdvbtee/libdvbtee/decode",
        "libdvbtee/libdvbtee/decode/table",
        "libdvbtee/libdvbtee/decode/descriptor",
        "<!(node -e \"require('nan')\")",
        "<!(node -e \"require('native-json')\")"
      ],
      'cflags': [ '-DDEBUG_CONSOLE=1' ],
      'cflags_cc': [ '-DDEBUG_CONSOLE=1', '-Wno-deprecated-declarations' ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions', '-Wdeprecated-declarations' ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'WARNING_CFLAGS': [
              '-Wno-deprecated-declarations'
            ],
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
    }
  ]
}
