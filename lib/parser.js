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
    self.options.enableEtt = (options.hasOwnProperty('enableEtt') ? options.enableEtt : true)
    self.options.enableParseISO6937 = (options.hasOwnProperty('enableParseISO6937') ? options.enableParseISO6937 : true)
  } else {
    self.options.passThru = false
    self.options.enableEtt = true
    self.options.enableParseISO6937 = true
  }

  var _parser       = new _Dvbtee.Parser

  var _reset        = _Dvbtee.Parser.prototype.reset
  self.reset        = _reset.bind(_parser)

  var _feed         = _Dvbtee.Parser.prototype.feed
  var feed          = _feed.bind(_parser)

  if (self.options.enableEtt) {
    /* libdvbtee has this feature disabled by default */
    var _enableEttCollection = _Dvbtee.Parser.prototype.enableEttCollection
    var enableEttCollection  = _enableEttCollection.bind(_parser)

    enableEttCollection(self.options.enableEtt)
  }

  if (!self.options.enableParseISO6937) {
    /* libdvbtee has this feature enabled by default */
    var _enableParseISO6937 = _Dvbtee.Parser.prototype.enableParseISO6937
    var enableParseISO6937  = _enableParseISO6937.bind(_parser)

    enableParseISO6937(self.options.enableParseISO6937)
  }

  stream.Transform.call(this, {
    objectMode: !self.options.passThru
  })

  self.bufferAllocUnsafe =
    (typeof(Buffer.allocUnsafe) === "function") ?
      Buffer.allocUnsafe : function(x) { return new Buffer(x) }

  self._buffer = self.bufferAllocUnsafe(0)

  var feed_progress = (self.options.passThru) ?
    function(a, b, c) {
      self.emit('psip', c)
    } : function(a, b, c) {
      self.push(c)
    }

  self.feed = function(b, blen, cb) {
    return feed(b, blen, feed_progress, cb)
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
    self._buffer = self.bufferAllocUnsafe(0)
  }
  done()
}

module.exports = Parser
