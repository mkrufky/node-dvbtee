var fs = require('fs')
var dvbtee = require('../')
var util = require('util')

var filename

if (process.argv.length <= 2) {
  filename = 'sample.ts'
} else {
  filename = process.argv[2]
}

var parser = new dvbtee.Parser()

parser.on('data', function(data) {
  console.log('\ntable data:', util.inspect(data, false, null))
})

fs.createReadStream(filename).pipe(parser)
