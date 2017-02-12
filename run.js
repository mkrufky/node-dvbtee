var dvbtee = require('./build/Release/dvbtee');

var fs = require('fs');

var parser = new dvbtee.Parser

parser.listenTables(function(tableId, tableName, d) {
  console.log('table id: ' + tableId,
              '\ntable name: ' + tableName,
              '\ntable data:\n', d)
})

fs.readFile('sample.ts', function(err, buf) {
    console.log('pushing ' + buf.byteLength + ' bytes');
    var ret = parser.push(buf, buf.byteLength, function(err, status) {
      if (err)
          console.log(err)
      else
          console.log('complete: status = ' + status)
      process.exit()
    })
})
