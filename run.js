var dvbtee = require('./build/Release/dvbtee');

var fs = require('fs');

var parser = (new dvbtee.Parser).listenTables(function(tableName, tableId, d) {
  console.log('table name: ' + tableName,
              'id: ' + tableId,
              '\n', d)
})


var cb = function(err, status) {
  if (err)
      console.log(err)
  else
      console.log('complete: status = ' + status)
}

fs.readFile('sample.ts', function(err, buf) {
    console.log('pushing ' + buf.byteLength + ' bytes returns:', parser.push(buf, buf.byteLength, cb) )
})
