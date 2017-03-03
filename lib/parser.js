var _Dvbtee = require('bindings')('dvbtee.node')
var util    = require('util')
var stream  = require('stream')

var Parser = function (options) {

  var self = this

  if (!(this instanceof Parser)) {
    return new Parser(options)
  }

  self.options = {}

  if (typeof options === 'object') {
    self.options.passThru = (options.hasOwnProperty('passThru') && options.passThru)
  } else {
    self.options.passThru = false
  }

  var _parser       = new _Dvbtee.Parser

  var _reset        = _Dvbtee.Parser.prototype.reset
  self.reset        = _reset.bind(_parser)

  var _feed         = _Dvbtee.Parser.prototype.feed
  self.feed         = _feed.bind(_parser)

  var _listenTables = _Dvbtee.Parser.prototype.listenTables
  var listenTables  = _listenTables.bind(_parser)

  stream.Transform.call(this, {
    objectMode: !self.options.passThru
  })

  self._buffer = new Buffer(0)

  if (self.options.passThru) {
    listenTables(function(a,b,c) {
      self.emit('psip', c)
    })
  } else {
    listenTables(function(a,b,c) {
      self.push(c)
    })
  }
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
        if (self.options.passThru) {
          self.push(b)
        }
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
      if (self.options.passThru) {
        self.push(b)
      }
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

Parser.prototype._flush = function (done) {

  var self = this

  // don't bother sending partial packets to the TS Parser- they'll just get dropped
  if ((self.options.passThru) && (self._buffer.length > 0)) {
    self.push(self._buffer)
    self._buffer = new Buffer(0)
  }
  done()
}

module.exports = Parser
