const fs = require('fs');
let raw = fs.readFileSync('test.json')
let out = JSON.parse(raw);
console.log(out[1]);


