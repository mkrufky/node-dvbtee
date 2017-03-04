var fs = require('fs')
var mkdirp = require('mkdirp')
var symlinkOrCopySync = require('symlink-or-copy').sync;

var configh = __dirname+'/../libdvbtee/libdvbpsi/config.h'

var confighContents = '#define HAVE_SYS_TYPES_H 1\n'+
                      '#define HAVE_INTTYPES_H 1\n'+
                      '#define HAVE_STDINT_H 1\n'

var libdvbpsiSrc = __dirname+'/../libdvbtee/libdvbpsi/src'
var libdvbpsiDst = __dirname+'/../libdvbtee/usr/include/dvbpsi/'

// for compat with node 0.12:
if (!String.prototype.hasOwnProperty('endsWith')) {
  String.prototype.endsWith = function(suffix) {
    return this.indexOf(suffix, this.length - suffix.length) !== -1
  }
}

fs.writeFile(configh, confighContents, function (err) {
  if (err) throw err

  mkdirp(libdvbpsiDst, function (err) {
    if (err) throw err;

    var cpHeaders = function (from) {
      fs.readdir(from, function (err, files) {
        if (err) throw err;
        files.forEach(function (file) {
          if (file.endsWith('.h')) {
            try {
              symlinkOrCopySync(from+'/'+file, libdvbpsiDst+file)
            } catch (e) {
              if (e.code !== 'EEXIST') throw e
            }
          }
        })
      })
    }

    cpHeaders(libdvbpsiSrc)
    cpHeaders(libdvbpsiSrc+'/tables')
    cpHeaders(libdvbpsiSrc+'/descriptors')
  })
})
