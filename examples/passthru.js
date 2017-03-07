var fs = require('fs')
var dvbtee = require('../')

var infilename
var outfilename

if (process.argv.length <= 2) {
  infilename = 'sample.ts'
} else {
  infilename = process.argv[2]
}

if (process.argv.length <= 3) {
  outfilename = 'output.ts'
} else {
  outfilename = process.argv[3]
}

var parser = dvbtee.Parser({'passThru': true})

parser.on('psip', function(data) {
  console.log('\ntable data:', data)
})

fs.createReadStream(infilename).pipe(parser).pipe(fs.createWriteStream(outfilename))
