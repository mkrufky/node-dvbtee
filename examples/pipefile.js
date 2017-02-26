var fs = require('fs')
var dvbtee = require('../')

var parser = dvbtee.Parser()

parser.on('data', function(data) {
  console.log('\ntable data:', data)
})

fs.createReadStream('sample.ts').pipe(parser)
