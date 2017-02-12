# node-dvbtee
[![Build Status](https://travis-ci.org/mkrufky/node-dvbtee.svg?branch=master)](https://travis-ci.org/mkrufky/node-dvbtee)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](#license)

node-dvbtee is a node.js addon for parsing mpeg2 TS transport streams containing broadcast PSIP tables

These tables contain information about the stream, such as broadcast info, program info, and electronic program guide (EPG).

Compilation requires both autotools and npm

## Installation

```bash
$ npm install dvbtee
```

## Example

```
var dvbtee = require('dvbtee');

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
```

## License
MIT License

Copyright (c) 2017 Michael Ira Krufky

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
