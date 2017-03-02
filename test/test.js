var assert = require('assert')
var should = require('should')
var dvbtee = require('../')

describe('node-dvbtee', function() {

  describe('require(\'dvbtee\')', function() {

    it('should return a function when called without operator new', function() {
      assert.equal("function", typeof require('../'))
    })

    it('should return a function when called as a function with operator new', function() {
      assert.equal("function", typeof new require('../'))
    })
  })

  describe('#libVersion()', function() {

    var version = dvbtee.libVersion()

    it('should return an Array of 3 Numbers to represent libdvbtee library version in the form [ MAJOR, MINOR, PATCH ]', function() {
      assert.equal(3, version.length)

      version.forEach(function (x) {
        assert.equal("number", typeof x)
      })
    })
  })

  describe('#logLevel()', function() {

    it('should return the dvbtee object (for method chaining)', function() {
      assert.equal(dvbtee, dvbtee.logLevel(0,0))
    })
  })

  describe('#getTableDecoderIds()', function() {

    var tableIds = dvbtee.getTableDecoderIds()

    it('should return an Array of Numbers that indicate the Table IDs supported by the built-in PSIP table decoders', function() {
      tableIds.forEach(function (x) {
        assert.equal("number", typeof x)
      })
    })

    it('should include 11 PSIP table decoders', function() {
      assert.equal(11, tableIds.length)
    })

    it('should support decoding PAT tables', function() {
      tableIds.should.containEql(0x00)
    })

    it('should support decoding PMT tables', function() {
      tableIds.should.containEql(0x02)
    })

    it('should support decoding NIT tables', function() {
      tableIds.should.containEql(0x40)
    })

    it('should support decoding SDT tables', function() {
      tableIds.should.containEql(0x42)
    })

    it('should support decoding EIT (dvb) tables', function() {
      tableIds.should.containEql(0x4e)
    })

    it('should support decoding TDT/TOT tables', function() {
      tableIds.should.containEql(0x70)
    })

    it('should support decoding MGT tables', function() {
      tableIds.should.containEql(0xc7)
    })

    it('should support decoding VCT tables', function() {
      tableIds.should.containEql(0xc8)
    })

    it('should support decoding EIT (atsc) tables', function() {
      tableIds.should.containEql(0xcb)
    })

    it('should support decoding ETT tables', function() {
      tableIds.should.containEql(0xcc)
    })

    it('should support decoding STT tables', function() {
      tableIds.should.containEql(0xcd)
    })
  })

  describe('#getDescriptorDecoderIds()', function() {

    var descIds = dvbtee.getDescriptorDecoderIds()

    it('should return an Array of Numbers that indicate the Descriptor IDs supported by the built-in PSIP descriptor decoders', function() {
      descIds.forEach(function (x) {
        assert.equal("number", typeof x)
      })
    })

    it('should include 9 PSIP descriptor decoders', function() {
      assert.equal(9, descIds.length)
    })

    it('should support decoding ISO639 language descriptors', function() {
      descIds.should.containEql(0x0a)
    })

    it('should support decoding service descriptors', function() {
      descIds.should.containEql(0x48)
    })

    it('should support decoding short event descriptors', function() {
      descIds.should.containEql(0x4d)
    })

    it('should support decoding frequency list descriptors', function() {
      descIds.should.containEql(0x62)
    })

    it('should support decoding AC-3 audio descriptors', function() {
      descIds.should.containEql(0x81)
    })

    it('should support decoding logical channel number descriptors', function() {
      descIds.should.containEql(0x83)
    })

    it('should support decoding caption service descriptors', function() {
      descIds.should.containEql(0x86)
    })

    it('should support decoding extended channel name descriptors', function() {
      descIds.should.containEql(0xa0)
    })

    it('should support decoding service location descriptors', function() {
      descIds.should.containEql(0xa1)
    })
  })
})

describe('dvbtee-parser', function() {

  describe('#New', function() {

    it('should return an object when called without operator new', function() {
      assert.equal("object", typeof dvbtee.Parser())
    })

    it('should return an object when called with operator new', function() {
      assert.equal("object", typeof new dvbtee.Parser())
    })

    it('correctly applies constructor option, \'passThru\' when set to true', function() {
      assert.equal(true, (new dvbtee.Parser({'passThru': true})).options.passThru )
    })

    it('correctly applies constructor option, \'passThru\' when set to true', function() {
      assert.equal(false, (new dvbtee.Parser({'passThru': false})).options.passThru )
    })

    it('correctly applies constructor option, \'passThru\' default setting to false when constructor options are omitted', function() {
      assert.equal(false, (new dvbtee.Parser).options.passThru )
    })
  })

/*
  describe('#listenTables()', function() {

    var parser = new dvbtee.Parser()

    it('should return the parser object (for method chaining)', function() {
      assert.equal(parser, parser.listenTables())
    })
  })
*/

  describe('#reset()', function() {

/*
    it('should return the parser object when called synchronously (without args) (for method chaining)', function() {
      var parser = new dvbtee.Parser()

      assert.equal(parser, parser.reset())
    })
*/

/*
    it('should return undefined when called asynchronously (with a callback function as the last argument)', function() {
      var parser = new dvbtee.Parser()

      assert.equal(undefined, parser.reset(function () { }))
    })
*/
  })

  describe('#feed()', function() {

    it('should return an error when called without args', function() {
      var parser = new dvbtee.Parser()

      assert.equal(-1, parser.feed())
    })

/*
    it('should return undefined when called asynchronously (with a callback function as the last argument)', function() {
      var parser = new dvbtee.Parser()

      assert.equal(undefined, parser.feed(function () { }))
    })
*/
  })
})
