# node-dvbtee
[![npm version](https://badge.fury.io/js/dvbtee.svg)](https://badge.fury.io/js/dvbtee)
[![Build Status](https://travis-ci.org/mkrufky/node-dvbtee.svg?branch=master)](https://travis-ci.org/mkrufky/node-dvbtee)
[![David](https://img.shields.io/david/mkrufky/node-dvbtee.svg)]()
[![David](https://img.shields.io/david/optional/mkrufky/node-dvbtee.svg)]()
[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](#license)

node-dvbtee is an MPEG2 transport stream parser addon for Node.js

Any standard MPEG2TS stream is supported, with additional specific support for broadcast television transport streams containing PSIP tables and descriptors.

These tables and descriptors contain information about the stream, such as broadcast info, program info, and electronic program guide (EPG).

[![NPM](https://nodei.co/npm/dvbtee.png?downloads=true&downloadRank=true)](https://www.npmjs.com/package/dvbtee)

## Installation

```bash
$ npm install dvbtee
```

## Example

```javascript
var dvbtee = require('dvbtee')

var parser = new dvbtee.Parser

parser.on('data', function(data) {
  console.log('table id: ' + data.tableId,
              '\ntable name: ' + data.tableName,
              '\ntable data:\n', JSON.stringify(data, null, 2))
})

// on a stream:
fs.createReadStream('sample1.ts').pipe(parser)

// on a buffer:
fs.readFile('sample2.ts', function(err, buf) {

    parser.feed(buf, buf.length, function(err, status) {

      if (err)
          console.log(err)
      else
          console.log('complete: status = ' + status)

    })
})
```

## Pass-Thru Mode Example

```javascript
var dvbtee = require('dvbtee')

var parser = new dvbtee.Parser({ 'passThru': true })

parser.on('psip', function(data) {
  console.log('table id: ' + data.tableId,
              '\ntable name: ' + data.tableName,
              '\ntable data:\n', JSON.stringify(data, null, 2))
})

// on a stream in pass-thru mode:
fs.createReadStream('sample3.ts').pipe(parser).pipe(fs.createWriteStream('outfile.ts'))
```

### Sample output from the above scripts
```
pushing 9029076 bytes
table id: 0 
table name: PAT 
table data:
 { programs: [ { number: 1, pid: 48 }, { number: 2, pid: 64 } ],
  tableId: 0,
  tableName: 'PAT',
  tsId: 2157,
  version: 2 }
table id: 199 
table name: MGT 
table data:
 { tableId: 199,
  tableName: 'MGT',
  tables: 
   [ { bytes: 120, pid: 8187, type: 0, version: 1 },
     { bytes: 409, pid: 5000, type: 256, version: 4 },
     { bytes: 621, pid: 5001, type: 257, version: 4 },
     { bytes: 431, pid: 5002, type: 258, version: 3 },
     { bytes: 398, pid: 5003, type: 259, version: 3 },
     { bytes: 308, pid: 5004, type: 260, version: 3 },
     { bytes: 534, pid: 5005, type: 261, version: 3 },
     { bytes: 434, pid: 5006, type: 262, version: 3 },
     { bytes: 560, pid: 5007, type: 263, version: 3 },
     { bytes: 365, pid: 5008, type: 264, version: 3 },
     { bytes: 561, pid: 5009, type: 265, version: 3 },
     { bytes: 446, pid: 5010, type: 266, version: 2 },
     { bytes: 388, pid: 5011, type: 267, version: 2 },
     { bytes: 297, pid: 5012, type: 268, version: 2 },
     { bytes: 500, pid: 5013, type: 269, version: 2 },
     { bytes: 423, pid: 5014, type: 270, version: 2 },
     { bytes: 526, pid: 5015, type: 271, version: 2 },
     { bytes: 421, pid: 5016, type: 272, version: 2 },
     { bytes: 536, pid: 5017, type: 273, version: 2 },
     { bytes: 430, pid: 5018, type: 274, version: 1 },
     { bytes: 354, pid: 5019, type: 275, version: 1 },
     { bytes: 280, pid: 5020, type: 276, version: 1 },
     { bytes: 572, pid: 5021, type: 277, version: 1 },
     { bytes: 406, pid: 5022, type: 278, version: 1 },
     { bytes: 598, pid: 5023, type: 279, version: 1 },
     { bytes: 719, pid: 5128, type: 512, version: 4 },
     { bytes: 708, pid: 5129, type: 513, version: 4 },
     { bytes: 700, pid: 5130, type: 514, version: 3 },
     { bytes: 478, pid: 5131, type: 515, version: 3 },
     { bytes: 511, pid: 5132, type: 516, version: 3 },
     { bytes: 356, pid: 5133, type: 517, version: 3 },
     { bytes: 667, pid: 5134, type: 518, version: 3 },
     { bytes: 399, pid: 5135, type: 519, version: 3 },
     { bytes: 571, pid: 5136, type: 520, version: 3 },
     { bytes: 544, pid: 5137, type: 521, version: 3 },
     { bytes: 591, pid: 5138, type: 522, version: 2 },
     { bytes: 293, pid: 5139, type: 523, version: 2 },
     { bytes: 490, pid: 5140, type: 524, version: 2 },
     { bytes: 364, pid: 5141, type: 525, version: 2 },
     { bytes: 677, pid: 5142, type: 526, version: 2 },
     { bytes: 409, pid: 5143, type: 527, version: 2 },
     { bytes: 760, pid: 5144, type: 528, version: 2 },
     { bytes: 576, pid: 5145, type: 529, version: 2 },
     { bytes: 645, pid: 5146, type: 530, version: 1 },
     { bytes: 314, pid: 5147, type: 531, version: 1 },
     { bytes: 463, pid: 5148, type: 532, version: 1 },
     { bytes: 377, pid: 5149, type: 533, version: 1 },
     { bytes: 542, pid: 5150, type: 534, version: 1 },
     { bytes: 451, pid: 5151, type: 535, version: 1 } ],
  version: 25 }
table id: 200 
table name: VCT 
table data:
 { channels: 
   [ { accessControlled: false,
       carrierFreq: 0,
       etmLocation: 0,
       hidden: false,
       hideGuide: false,
       major: 2,
       minor: 1,
       modulation: 4,
       outOfBand: true,
       pathSelect: true,
       program: 1,
       serviceName: 'WCBS-HD',
       serviceType: 2,
       sourceId: 1,
       tsId: 2157 },
     { accessControlled: false,
       carrierFreq: 0,
       etmLocation: 0,
       hidden: false,
       hideGuide: false,
       major: 2,
       minor: 2,
       modulation: 4,
       outOfBand: true,
       pathSelect: true,
       program: 2,
       serviceName: 'DECADES',
       serviceType: 2,
       sourceId: 2,
       tsId: 2157 } ],
  isCableVCT: false,
  tableId: 200,
  tableName: 'VCT',
  tsId: 2157,
  version: 1 }
table id: 2 
table name: PMT 
table data:
 { pcrPid: 49,
  program: 1,
  streams: 
   [ { pid: 49, streamType: 2, streamTypeString: 'Video MPEG-2' },
     { pid: 52,
       streamType: 129,
       streamTypeString: 'Audio AC3 (ATSC)' },
     { pid: 53,
       streamType: 129,
       streamTypeString: 'Audio AC3 (ATSC)' } ],
  tableId: 2,
  tableName: 'PMT',
  version: 1 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5083,
       lengthSec: 1800,
       startTime: 1137801616,
       title: 'The Insider' },
     { etmLoc: 1,
       eventId: 5084,
       lengthSec: 1800,
       startTime: 1137803416,
       title: 'Entertainment Tonight' },
     { etmLoc: 1,
       eventId: 5085,
       lengthSec: 3600,
       startTime: 1137805216,
       title: 'Supergirl' },
     { etmLoc: 1,
       eventId: 5086,
       lengthSec: 3540,
       startTime: 1137808816,
       title: 'Scorpion' },
     { etmLoc: 1,
       eventId: 5087,
       lengthSec: 3660,
       startTime: 1137812356,
       title: 'NCIS: Los Angeles' } ],
  extension: 1,
  sourceId: 1,
  tableId: 203,
  tableName: 'EIT',
  version: 4 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5156,
       lengthSec: 7200,
       startTime: 1138017616,
       title: 'CBS This Morning' },
     { etmLoc: 1,
       eventId: 5157,
       lengthSec: 1800,
       startTime: 1138024816,
       title: 'Hot Bench' },
     { etmLoc: 1,
       eventId: 5158,
       lengthSec: 1800,
       startTime: 1138026616,
       title: 'Hot Bench' } ],
  extension: 1,
  sourceId: 1,
  tableId: 203,
  tableName: 'EIT',
  version: 1 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5081,
       lengthSec: 3600,
       startTime: 1137801616,
       title: 'Through the Decades' },
     { etmLoc: 1,
       eventId: 5082,
       lengthSec: 7200,
       startTime: 1137805216,
       title: 'The Day Reagan Was Shot' } ],
  extension: 2,
  sourceId: 2,
  tableId: 203,
  tableName: 'EIT',
  version: 4 }
table id: 2 
table name: PMT 
table data:
 { pcrPid: 65,
  program: 2,
  streams: 
   [ { pid: 65, streamType: 2, streamTypeString: 'Video MPEG-2' },
     { pid: 68,
       streamType: 129,
       streamTypeString: 'Audio AC3 (ATSC)' } ],
  tableId: 2,
  tableName: 'PMT',
  version: 1 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5145,
       lengthSec: 3600,
       startTime: 1138017616,
       title: 'Through the Decades' },
     { etmLoc: 1,
       eventId: 5146,
       lengthSec: 9000,
       startTime: 1138021216,
       title: 'Club Land' } ],
  extension: 2,
  sourceId: 2,
  tableId: 203,
  tableName: 'EIT',
  version: 1 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 0,
       eventId: 5159,
       lengthSec: 3600,
       startTime: 1138028416,
       title: 'Let\'s Make a Deal' },
     { etmLoc: 0,
       eventId: 5160,
       lengthSec: 3600,
       startTime: 1138032016,
       title: 'The Price Is Right' },
     { etmLoc: 1,
       eventId: 5161,
       lengthSec: 1800,
       startTime: 1138035616,
       title: 'CBS 2 News at Noon' },
     { etmLoc: 1,
       eventId: 5162,
       lengthSec: 3600,
       startTime: 1138037416,
       title: 'The Young and the Restless' } ],
  extension: 1,
  sourceId: 1,
  tableId: 203,
  tableName: 'EIT',
  version: 1 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5146,
       lengthSec: 9000,
       startTime: 1138021216,
       title: 'Club Land' },
     { etmLoc: 0,
       eventId: 5147,
       lengthSec: 1800,
       startTime: 1138030216,
       title: 'MovieStar' },
     { etmLoc: 1,
       eventId: 5148,
       lengthSec: 3600,
       startTime: 1138032016,
       title: 'Kent State: The Day the War Came Home' },
     { etmLoc: 1,
       eventId: 5149,
       lengthSec: 1800,
       startTime: 1138035616,
       title: 'Power Players' },
     { etmLoc: 0,
       eventId: 5150,
       lengthSec: 1800,
       startTime: 1138037416,
       title: 'Power Players' } ],
  extension: 2,
  sourceId: 2,
  tableId: 203,
  tableName: 'EIT',
  version: 1 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5162,
       lengthSec: 3600,
       startTime: 1138037416,
       title: 'The Young and the Restless' },
     { etmLoc: 1,
       eventId: 5163,
       lengthSec: 1800,
       startTime: 1138041016,
       title: 'The Bold and the Beautiful' },
     { etmLoc: 1,
       eventId: 5164,
       lengthSec: 3600,
       startTime: 1138042816,
       title: 'The Talk' },
     { etmLoc: 1,
       eventId: 5165,
       lengthSec: 3600,
       startTime: 1138046416,
       title: 'Dr. Phil' } ],
  extension: 1,
  sourceId: 1,
  tableId: 203,
  tableName: 'EIT',
  version: 1 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5151,
       lengthSec: 3600,
       startTime: 1138039216,
       title: 'Through the Decades' },
     { etmLoc: 1,
       eventId: 5152,
       lengthSec: 9000,
       startTime: 1138042816,
       title: 'Club Land' } ],
  extension: 2,
  sourceId: 2,
  tableId: 203,
  tableName: 'EIT',
  version: 1 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 0,
       eventId: 5166,
       lengthSec: 1800,
       startTime: 1138050016,
       title: 'Judge Judy' },
     { etmLoc: 1,
       eventId: 5167,
       lengthSec: 1800,
       startTime: 1138051816,
       title: 'Judge Judy' },
     { etmLoc: 1,
       eventId: 5168,
       lengthSec: 3600,
       startTime: 1138053616,
       title: 'CBS 2 News at 5PM' },
     { etmLoc: 1,
       eventId: 5169,
       lengthSec: 1800,
       startTime: 1138057216,
       title: 'CBS 2 News at 6PM' },
     { etmLoc: 0,
       eventId: 5170,
       lengthSec: 1800,
       startTime: 1138059016,
       title: 'CBS Evening News With Scott Pelley' } ],
  extension: 1,
  sourceId: 1,
  tableId: 203,
  tableName: 'EIT',
  version: 1 }
table id: 205 
table name: STT 
table data:
 { descriptors: [],
  tableId: 205,
  tableName: 'STT',
  time: 1453775318 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5152,
       lengthSec: 9000,
       startTime: 1138042816,
       title: 'Club Land' },
     { etmLoc: 0,
       eventId: 5153,
       lengthSec: 1800,
       startTime: 1138051816,
       title: 'MovieStar' },
     { etmLoc: 1,
       eventId: 5154,
       lengthSec: 3600,
       startTime: 1138053616,
       title: 'Kent State: The Day the War Came Home' },
     { etmLoc: 1,
       eventId: 5155,
       lengthSec: 1800,
       startTime: 1138057216,
       title: 'Power Players' },
     { etmLoc: 0,
       eventId: 5156,
       lengthSec: 1800,
       startTime: 1138059016,
       title: 'Power Players' } ],
  extension: 2,
  sourceId: 2,
  tableId: 203,
  tableName: 'EIT',
  version: 1 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5090,
       lengthSec: 3600,
       startTime: 1137821836,
       title: 'The Late Late Show With James Corden' },
     { etmLoc: 1,
       eventId: 5091,
       lengthSec: 1800,
       startTime: 1137825436,
       title: 'Comics Unleashed With Byron Allen' },
     { etmLoc: 1,
       eventId: 5092,
       lengthSec: 1800,
       startTime: 1137827236,
       title: 'Fast Joint Relief' },
     { etmLoc: 1,
       eventId: 5093,
       lengthSec: 1800,
       startTime: 1137829036,
       title: 'Larry King Special Report' },
     { etmLoc: 1,
       eventId: 5094,
       lengthSec: 3180,
       startTime: 1137830836,
       title: 'CBS Overnight News' } ],
  extension: 1,
  sourceId: 1,
  tableId: 203,
  tableName: 'EIT',
  version: 3 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5089,
       lengthSec: 3600,
       startTime: 1137823216,
       title: 'Through the Decades' },
     { etmLoc: 1,
       eventId: 5090,
       lengthSec: 7200,
       startTime: 1137826816,
       title: 'The Day Reagan Was Shot' } ],
  extension: 2,
  sourceId: 2,
  tableId: 203,
  tableName: 'EIT',
  version: 3 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5095,
       lengthSec: 1800,
       startTime: 1137834016,
       title: 'CBS Morning News' },
     { etmLoc: 1,
       eventId: 5096,
       lengthSec: 9000,
       startTime: 1137835816,
       title: 'CBS 2 News This Morning' } ],
  extension: 1,
  sourceId: 1,
  tableId: 203,
  tableName: 'EIT',
  version: 3 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5091,
       lengthSec: 1800,
       startTime: 1137834016,
       title: 'The Greats' },
     { etmLoc: 1,
       eventId: 5092,
       lengthSec: 1800,
       startTime: 1137835816,
       title: 'The Greats' },
     { etmLoc: 0,
       eventId: 5093,
       lengthSec: 1800,
       startTime: 1137837616,
       title: 'Power Players' },
     { etmLoc: 0,
       eventId: 5094,
       lengthSec: 1800,
       startTime: 1137839416,
       title: 'Power Players' },
     { etmLoc: 1,
       eventId: 5095,
       lengthSec: 1800,
       startTime: 1137841216,
       title: 'The Achievers' },
     { etmLoc: 1,
       eventId: 5096,
       lengthSec: 1800,
       startTime: 1137843016,
       title: 'M*A*S*H' } ],
  extension: 2,
  sourceId: 2,
  tableId: 203,
  tableName: 'EIT',
  version: 3 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5087,
       lengthSec: 3660,
       startTime: 1137812356,
       title: 'NCIS: Los Angeles' },
     { etmLoc: 1,
       eventId: 5088,
       lengthSec: 2100,
       startTime: 1137816016,
       title: 'CBS 2 News at 11PM' },
     { etmLoc: 1,
       eventId: 5089,
       lengthSec: 3720,
       startTime: 1137818116,
       title: 'The Late Show With Stephen Colbert' },
     { etmLoc: 1,
       eventId: 5090,
       lengthSec: 3600,
       startTime: 1137821836,
       title: 'The Late Late Show With James Corden' } ],
  extension: 1,
  sourceId: 1,
  tableId: 203,
  tableName: 'EIT',
  version: 4 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5097,
       lengthSec: 7200,
       startTime: 1137844816,
       title: 'CBS This Morning' },
     { etmLoc: 1,
       eventId: 5098,
       lengthSec: 1800,
       startTime: 1137852016,
       title: 'Hot Bench' },
     { etmLoc: 1,
       eventId: 5099,
       lengthSec: 1800,
       startTime: 1137853816,
       title: 'Hot Bench' } ],
  extension: 1,
  sourceId: 1,
  tableId: 203,
  tableName: 'EIT',
  version: 3 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5083,
       lengthSec: 1800,
       startTime: 1137812416,
       title: 'The Greats' },
     { etmLoc: 1,
       eventId: 5084,
       lengthSec: 1800,
       startTime: 1137814216,
       title: 'The Greats' },
     { etmLoc: 0,
       eventId: 5085,
       lengthSec: 1800,
       startTime: 1137816016,
       title: 'Power Players' },
     { etmLoc: 0,
       eventId: 5086,
       lengthSec: 1800,
       startTime: 1137817816,
       title: 'Power Players' },
     { etmLoc: 1,
       eventId: 5087,
       lengthSec: 1800,
       startTime: 1137819616,
       title: 'The Achievers' },
     { etmLoc: 1,
       eventId: 5088,
       lengthSec: 1800,
       startTime: 1137821416,
       title: 'M*A*S*H' } ],
  extension: 2,
  sourceId: 2,
  tableId: 203,
  tableName: 'EIT',
  version: 4 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5097,
       lengthSec: 3600,
       startTime: 1137844816,
       title: 'Through the Decades' },
     { etmLoc: 1,
       eventId: 5098,
       lengthSec: 7200,
       startTime: 1137848416,
       title: 'Ann Rule Presents: The Stranger Beside Me' } ],
  extension: 2,
  sourceId: 2,
  tableId: 203,
  tableName: 'EIT',
  version: 3 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5119,
       lengthSec: 3600,
       startTime: 1137908236,
       title: 'The Late Late Show With James Corden' },
     { etmLoc: 1,
       eventId: 5120,
       lengthSec: 1800,
       startTime: 1137911836,
       title: 'Comics Unleashed With Byron Allen' },
     { etmLoc: 1,
       eventId: 5121,
       lengthSec: 1800,
       startTime: 1137913636,
       title: 'Paid Programming' },
     { etmLoc: 1,
       eventId: 5122,
       lengthSec: 1800,
       startTime: 1137915436,
       title: 'How to Avoid a Facelift' },
     { etmLoc: 1,
       eventId: 5123,
       lengthSec: 3180,
       startTime: 1137917236,
       title: 'CBS Overnight News' } ],
  extension: 1,
  sourceId: 1,
  tableId: 203,
  tableName: 'EIT',
  version: 2 }
table id: 203 
table name: EIT 
table data:
 { events: 
   [ { etmLoc: 1,
       eventId: 5115,
       lengthSec: 3600,
       startTime: 1137909616,
       title: 'Through the Decades' },
     { etmLoc: 1,
       eventId: 5116,
       lengthSec: 7200,
       startTime: 1137913216,
       title: 'Ann Rule Presents: The Stranger Beside Me' } ],
  extension: 2,
  sourceId: 2,
  tableId: 203,
  tableName: 'EIT',
  version: 2 }
complete: status = 0
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

[![Analytics](https://ga-beacon.appspot.com/UA-71301363-4/mkrufky/node-dvbtee/README.md)](https://github.com/igrigorik/ga-beacon)
