var fs = require('fs')
var dvbtee = require('../build/Release/dvbtee.node')

var filename

if (process.argv.length <= 2) {
  filename = 'sample.ts'
} else {
  filename = process.argv[2]
}

var parser = new dvbtee.Parser

parser.listenTables(function(a,b,data) {
  console.log('\ntable data:\n', data)
})

fs.readFile(filename, function(err, buf) {

    console.log('pushing ' + buf.length + ' bytes')

    parser.feed(buf, buf.length, function(err, status) {

      if (err)
          console.log(err)
      else
          console.log('complete: status = ' + status)
    })
})
