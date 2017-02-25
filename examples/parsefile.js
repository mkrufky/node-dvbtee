var fs = require('fs')
var dvbtee = require('bindings')('dvbtee.node')

var parser = new dvbtee.Parser

parser.listenTables(function(tableId, tableName, data) {

  console.log('table id: ' + tableId,
              '\ntable name: ' + tableName,
              '\ntable data:\n', JSON.stringify(data, null, 2))
})

fs.readFile('sample.ts', function(err, buf) {

    console.log('pushing ' + buf.length + ' bytes')

    parser.feed(buf, buf.length, function(err, status) {

      if (err)
          console.log(err)
      else
          console.log('complete: status = ' + status)
    })
})
