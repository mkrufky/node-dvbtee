var fs = require('fs')
var dvbtee = require('../')

var filename

if (process.argv.length <= 2) {
  filename = 'sample.ts'
} else {
  filename = process.argv[2]
}

var parser = dvbtee.Parser()

parser.on('data', function(data) {
  console.log('\ntable data:', data)
})

fs.createReadStream(filename).pipe(parser)
