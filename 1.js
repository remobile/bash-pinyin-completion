var a = 'a';
var b = 'b';
var c = 'c';
var d = 'd';
var e = 'e';

var _ = require('lodash');

function getValue(list, value) {
    let str = '';
    for (const i in value) {
        str += list[i][value[i]];
    }
    return str;
}

function addValue(value, list, n) {
    if (n >= 0) {
        value[n]++;
        if (value[n] == list[n].length) {
            value[n] = 0;
            addValue(value, list, n-1);
        }
    }
}

var list = [a,b,c,d,e];
var value = _.fill(Array(5), 0);
for (var i=0; i<1; i++) {
    console.log("=======", getValue(list, value));
    addValue(value, list, 4);
}
