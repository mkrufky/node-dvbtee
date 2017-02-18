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
      '../libdvbtee/libdvbpsi/src',
      '../libdvbtee/libdvbpsi/src/tables',
      '../libdvbtee/libdvbpsi/src/descriptors',
      '../libdvbtee/libdvbpsi'
    ],
    'defines': [
      'PIC',
      'HAVE_CONFIG_H'
    ],
  },

  'targets': [
    # libdvbpsi
    {
      'target_name': 'dvbpsi',
      'product_prefix': 'lib',
      'type': 'static_library',
      'sources': [
        '../libdvbtee/libdvbpsi/src/dvbpsi.c',
        '../libdvbtee/libdvbpsi/src/psi.c',
        '../libdvbtee/libdvbpsi/src/demux.c',
        '../libdvbtee/libdvbpsi/src/descriptor.c',
        '../libdvbtee/libdvbpsi/src/tables/pat.c',
        '../libdvbtee/libdvbpsi/src/tables/pmt.c',
        '../libdvbtee/libdvbpsi/src/tables/sdt.c',
        '../libdvbtee/libdvbpsi/src/tables/eit.c',
#       '../libdvbtee/libdvbpsi/src/tables/cat.c',
        '../libdvbtee/libdvbpsi/src/tables/nit.c',
        '../libdvbtee/libdvbpsi/src/tables/tot.c',
#       '../libdvbtee/libdvbpsi/src/tables/sis.c',
#       '../libdvbtee/libdvbpsi/src/tables/bat.c',
#       '../libdvbtee/libdvbpsi/src/tables/rst.c',
        '../libdvbtee/libdvbpsi/src/tables/atsc_vct.c',
        '../libdvbtee/libdvbpsi/src/tables/atsc_stt.c',
        '../libdvbtee/libdvbpsi/src/tables/atsc_eit.c',
        '../libdvbtee/libdvbpsi/src/tables/atsc_ett.c',
        '../libdvbtee/libdvbpsi/src/tables/atsc_mgt.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_02.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_03.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_04.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_05.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_06.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_07.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_08.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_09.c',
        '../libdvbtee/libdvbpsi/src/descriptors/dr_0a.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_0b.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_0c.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_0d.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_0e.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_0f.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_10.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_11.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_12.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_13.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_14.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_1b.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_1c.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_24.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_40.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_41.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_42.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_43.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_44.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_45.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_47.c',
        '../libdvbtee/libdvbpsi/src/descriptors/dr_48.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_49.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_4a.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_4b.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_4c.c',
        '../libdvbtee/libdvbpsi/src/descriptors/dr_4d.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_4e.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_4f.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_50.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_52.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_53.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_54.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_55.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_56.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_58.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_59.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_5a.c',
        '../libdvbtee/libdvbpsi/src/descriptors/dr_62.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_66.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_69.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_73.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_76.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_7c.c',
        '../libdvbtee/libdvbpsi/src/descriptors/dr_81.c',
        '../libdvbtee/libdvbpsi/src/descriptors/dr_83.c',
        '../libdvbtee/libdvbpsi/src/descriptors/dr_86.c',
#       '../libdvbtee/libdvbpsi/src/descriptors/dr_8a.c',
        '../libdvbtee/libdvbpsi/src/descriptors/dr_a0.c',
        '../libdvbtee/libdvbpsi/src/descriptors/dr_a1.c',
      ],
      'conditions': [
        ['OS=="mac"',
          {
            'xcode_settings': {
              'WARNING_CFLAGS': [
                '-Wno-deprecated-declarations'
              ]
            }
          }
        ]
      ],
      'cflags!': ['-Wdeprecated-declarations','-Wimplicit-function-declaration'],
      'cflags+': ['-Wno-deprecated-declarations','-Wno-implicit-function-declaration','-std=c99'],
    },
  ]
}
