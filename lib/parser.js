var _Dvbtee = require('bindings')('dvbtee.node')
var util    = require('util')
var stream  = require('stream')

var Parser = function () {

  var self = this

  if (!(this instanceof Parser)) {
    return new Parser()
  }

  var _parser        = new _Dvbtee.Parser

  var _reset         = _Dvbtee.Parser.prototype.reset
  self.reset         = _reset.bind(_parser)

  var _feed          = _Dvbtee.Parser.prototype.feed
  self.feed          = _feed.bind(_parser)

  var _listenTables  = _Dvbtee.Parser.prototype.listenTables
  var __listenTables = _listenTables.bind(_parser)

  var _streamPush    = stream.Transform.prototype.push
  var __streamPush   = _streamPush.bind(self)

  _Dvbtee.Parser.call(_parser)

  stream.Transform.call(this, {
    objectMode: true
  })

  self._buffer = new Buffer(0)

  __listenTables(function(a,b,c) {
    __streamPush(c)
  })
}

util.inherits(Parser, stream.Transform)

Parser.prototype._transform = function (chunk, encoding, done) {

  var self = this

  var checkCache = function (cb) {
    if (self._buffer.length + chunk.length < 188) {
      try {
        self._buffer = Buffer.concat([ self._buffer, chunk ])
        done()
      } catch (err) {
        done(err)
      }
    } else {
      cb()
    }
  }

  var feedCache = function (cb) {

    if (self._buffer.length > 0) {
      try {
        var b = Buffer.concat(
          [ self._buffer, chunk.slice(0, 188 - self._buffer.length) ], 188)
        self.feed(b, b.length, function (err) {
          chunk = chunk.slice(188 - self._buffer.length)
          cb(err)
        })
      } catch (err) {
        cb(err)
      }
    } else {
      cb()
    }
  }

  var feedChunk = function (cb) {

    try {
      var next = 188 * Math.floor(chunk.length / 188)
      var b = chunk.slice(0, next)

      self.feed(b, b.length, function (err) {
        self._buffer = chunk.slice(next)
        cb(err)
      })
    } catch (err) {
      cb(err)
    }
  }

  checkCache(function () {
    feedCache(function (err) {
      if (err) {
        done(err)
      } else {
        feedChunk(done)
      }
    })
  })
}

module.exports = Parser
