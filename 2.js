var a = ['a', 'b'];
var b = ['1', '2', '3'];
var c = ['A', 'B'];
var d = ['1', '2', '3'];

var size = 12*3;



var arr = [a, b, c, d];
var list = [];
var countList = [2, 3, 2, 3];

function fang(arr, countList, level) {
    var preCount = 0;
    for (var i=0; i<level; i++) {
        preCount += countList[i];
    }
    preCount = preCount || 1;
    var currentCount = countList[level];

    for (var i=0; i<size; i++) {
        list[i] = `${list[i]||''}${arr[level][Math.floor(i/preCount)%currentCount]}`;
    }
}

for (var i=0; i<4; i++) {
    fang(arr, countList, i);
}
console.log("=======", list);
