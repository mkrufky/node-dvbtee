var fs = require('fs')
var dvbtee = require('../')

var parser = dvbtee.Parser({'passThru': true})

parser.on('psip', function(data) {
  console.log('\ntable data:', data)
})

fs.createReadStream('sample.ts').pipe(parser).pipe(fs.createWriteStream('output.ts'))
