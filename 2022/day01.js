
const fs = require('fs');

function readFileToArray(filename) {
    const contents = fs.readFileSync(filename, 'utf-8');
    const arr = contents.split(/\r?\n/);
    return arr;
}

let input_data = readFileToArray('./data/day01.txt');
console.log(input_data);

let sums = [0]
let elfNr = 0
for (let i = 0; i < input_data.length; i++) {
    if(input_data[i] == ''){
        elfNr += 1
        sums[elfNr] = 0
    }
    else
        sums[elfNr] += Number(input_data[i])
}

let highestSum = Math.max(...sums) 
let highestElf = sums.indexOf(highestSum) + 1

sums.sort(function(a, b){return a - b}).reverse();

let sumTop3 = sums[0]+sums[1]+sums[2]   // 206104
console.log(`1: ${sums[0]} --- 2: ${sums[1]} --- 3: ${sums[2]} \t sumTop3:${sumTop3} \t highestElf:${highestElf}`)
// 1: 69310 --- 2: 68501 --- 3: 68293       sumTop3:206104          highestElf:178