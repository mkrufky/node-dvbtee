{
  "targets": [
    {
      "target_name": "dvbtee",
      "sources": [ "addon.cc", "dvbtee-parser.cc" ],
      "include_dirs": [
        "libdvbtee/usr/include/",
        "libdvbtee/usr/include/dvbtee",
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [
        "../libdvbtee/usr/lib/libdvbtee.so"
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
