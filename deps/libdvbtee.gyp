{
  'target_defaults': {
    'default_configuration': 'Debug',
    'configurations': {
      'Debug': {
        'defines': [ 'DEBUG', '_DEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 1, # static debug
          },
        },
      },
      'Release': {
        'defines': [ 'NDEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 0, # static release
          },
        },
      }
    },
    'msvs_settings': {
      'VCLinkerTool': {
        'GenerateDebugInformation': 'true',
      },
    },
    'include_dirs': [
      '../libdvbtee/usr/include',
      '../libdvbtee/usr/include/dvbtee',
      '../libdvbtee/usr/include/dvbpsi',
      '../libdvbtee/libdvbtee',
      '../libdvbtee/libdvbtee/decode',
      '../libdvbtee/libdvbtee/decode/table',
      '../libdvbtee/libdvbtee/decode/descriptor',
      '../libdvbtee'
    ],
    'defines': [
      'PIC',
      'FORCE_DECODER_LINKAGE'
    ],
  },

  'targets': [
    # libdvbtee_parser
    {
      'target_name': 'dvbtee_parser',
      'product_prefix': 'lib',
      'type': 'static_library',
      'sources': [
        '../libdvbtee/libdvbtee/atsctext.cpp',
#       '../libdvbtee/libdvbtee/channels.cpp',
#       '../libdvbtee/libdvbtee/curlhttpget.cpp',
        '../libdvbtee/libdvbtee/decode.cpp',
#       '../libdvbtee/libdvbtee/demux.cpp',
#       '../libdvbtee/libdvbtee/feed.cpp',
        '../libdvbtee/libdvbtee/functions.cpp',
#       '../libdvbtee/libdvbtee/hdhr_tuner.cpp',
#       '../libdvbtee/libdvbtee/hlsfeed.cpp',
#       '../libdvbtee/libdvbtee/linuxtv_tuner.cpp',
#       '../libdvbtee/libdvbtee/listen.cpp',
        '../libdvbtee/libdvbtee/log.cpp',
#       '../libdvbtee/libdvbtee/output.cpp',
        '../libdvbtee/libdvbtee/parse.cpp',
#       '../libdvbtee/libdvbtee/rbuf.cpp',
        '../libdvbtee/libdvbtee/stats.cpp',
#       '../libdvbtee/libdvbtee/tune.cpp',

        '../libdvbtee/libdvbtee/value/value.cpp',
        '../libdvbtee/libdvbtee/value/object.cpp',
        '../libdvbtee/libdvbtee/value/array.cpp',

        '../libdvbtee/libdvbtee/decode/decoder.cpp',
        '../libdvbtee/libdvbtee/decode/table/table.cpp',
        '../libdvbtee/libdvbtee/decode/descriptor/descriptor.cpp',

        '../libdvbtee/libdvbtee/decode/table/tabl_00.cpp',
        '../libdvbtee/libdvbtee/decode/table/tabl_02.cpp',
        '../libdvbtee/libdvbtee/decode/table/tabl_40.cpp',
        '../libdvbtee/libdvbtee/decode/table/tabl_42.cpp',
        '../libdvbtee/libdvbtee/decode/table/tabl_4e.cpp',
        '../libdvbtee/libdvbtee/decode/table/tabl_70.cpp',
        '../libdvbtee/libdvbtee/decode/table/tabl_c7.cpp',
        '../libdvbtee/libdvbtee/decode/table/tabl_c8.cpp',
        '../libdvbtee/libdvbtee/decode/table/tabl_cb.cpp',
        '../libdvbtee/libdvbtee/decode/table/tabl_cc.cpp',
        '../libdvbtee/libdvbtee/decode/table/tabl_cd.cpp',

        '../libdvbtee/libdvbtee/decode/descriptor/desc_0a.cpp',
        '../libdvbtee/libdvbtee/decode/descriptor/desc_48.cpp',
        '../libdvbtee/libdvbtee/decode/descriptor/desc_4d.cpp',

        '../libdvbtee/libdvbtee/decode/descriptor/desc_62.cpp',
        '../libdvbtee/libdvbtee/decode/descriptor/desc_83.cpp',

        '../libdvbtee/libdvbtee/decode/descriptor/desc_81.cpp',
        '../libdvbtee/libdvbtee/decode/descriptor/desc_86.cpp',
        '../libdvbtee/libdvbtee/decode/descriptor/desc_a0.cpp',
        '../libdvbtee/libdvbtee/decode/descriptor/desc_a1.cpp',
      ],
      "dependencies": [
        'libdvbpsi.gyp:dvbpsi'
      ],
      'conditions': [
        ['OS=="mac"',
          {
            'link_settings': {
              'libraries': [
                '-liconv'
              ]
            },
            'xcode_settings': {
              'OTHER_LDFLAGS': [
                '-L/opt/local/lib'
              ],
              'WARNING_CFLAGS': [
                '-Wno-unused-variable',
                '-Wno-deprecated-declarations'
              ],
              'GCC_ENABLE_CPP_RTTI': '-frtti'
            }
          }
        ]
      ],
      'cflags_cc!': ['-fno-rtti','-Wunused-variable','-Wdeprecated-declarations'],
      'cflags_cc+': ['-frtti','-Wno-unused-variable','-Wno-deprecated-declarations'],
    },

    # dvbtee command line application
    {
      'target_name': 'dvbteee',
      'type': 'executable',
      'dependencies': [ 'dvbtee_parser' ],
      'sources': [ '../libdvbtee/dvbtee/dvbtee.cpp' ]
    },
  ]
}
