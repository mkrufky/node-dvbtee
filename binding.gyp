{
  "targets": [
    {
      "target_name": "dvbtee",
      "sources": [ "addon.cc", "dvbtee-parser.cc" ],
      "dependencies": [
        'libdvbtee.gyp:dvbtee_parser'
      ],
      "include_dirs": [
        "libdvbtee/usr/include",
        "libdvbtee/libdvbtee",
        "libdvbtee/libdvbtee/decode",
        "libdvbtee/libdvbtee/decode/table",
        "libdvbtee/libdvbtee/decode/descriptor",
        "<!(node -e \"require('nan')\")"
      ],
      'cflags': [ '-DDEBUG_CONSOLE=1' ],
      'cflags_cc': [ '-DDEBUG_CONSOLE=1' ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
    }
  ]
}
