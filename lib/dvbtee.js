var _Dvbtee = require('bindings')('dvbtee.node')
var Parser  = require('./parser.js')

function Dvbtee () {
  if (!(this instanceof Dvbtee)) {
    return new Dvbtee()
  }
  _Dvbtee.call(this)
}

Dvbtee.logLevel                = _Dvbtee.logLevel
Dvbtee.libVersion              = _Dvbtee.libVersion
Dvbtee.getTableDecoderIds      = _Dvbtee.getTableDecoderIds
Dvbtee.getDescriptorDecoderIds = _Dvbtee.getDescriptorDecoderIds

Dvbtee.Parser                  = Parser

module.exports = Dvbtee
