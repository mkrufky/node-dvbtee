{
  "targets": [
    {
      "target_name": "dvbtee",
      "sources": [
        "src/node-dvbtee.cc",
        "src/dvbtee-parser.cc"
      ],
      "dependencies": [
        "<!(node -e \"require('libdvbtee_parser')\")/binding.gyp:dvbtee_parser"
      ],
      "include_dirs": [
        "<!(node -e \"require('libdvbtee_parser')\")/usr/include",
        "<!(node -e \"require('libdvbtee_parser')\")/libdvbtee",
        "<!(node -e \"require('libdvbtee_parser')\")/libdvbtee/decode",
        "<!(node -e \"require('libdvbtee_parser')\")/libdvbtee/decode/table",
        "<!(node -e \"require('libdvbtee_parser')\")/libdvbtee/decode/descriptor",
        "<!(node -e \"require('async-factory-worker')\")",
        "<!(node -e \"require('nan')\")"
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
