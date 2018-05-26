var clone = require('git-clone')
var fs = require('fs')
var rimraf = require('rimraf')

var libdvbteeDir = __dirname+'/../libdvbtee'
var libdvbpsiDir = __dirname+'/../libdvbtee/libdvbpsi'

var cloneLibdvbtee = function(cb) {
    return fs.stat(libdvbteeDir, function(err, stats) {
        if (err) {
            return clone('git://github.com/mkrufky/libdvbtee.git', libdvbteeDir, {'shallow': true}, cb)
        } else {
            return cb()
        }
    })
}

var cloneLibdvbpsi = function() {
    return fs.stat(libdvbpsiDir+'/bootstrap', function(err, stats) {
        if (err) {
            return rimraf(libdvbpsiDir, function() {
                return clone('git://github.com/mkrufky/libdvbpsi.git', libdvbpsiDir, {'shallow': true}, function() {
                    return fs.writeFileSync(libdvbpsiDir+'.dont_del', 0)
                })
            })
        }
    })
}

cloneLibdvbtee(cloneLibdvbpsi)
