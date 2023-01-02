

let utils = require('./utils.js');
const t0 = performance.now();
console.log(utils.currentDateTime(), t0)
let day = 13
let input_data = utils.readFileToArray(`./data/day${day}.txt`, 1, /\r\n\s*\r\n/);   /// \n\s*\n/   // /\r\n\s*\r\n/
//console.table(input_data)
let pairs = input_data.map((i) => i.split('\r\n').map((y) => JSON.parse(y)));
//console.table(pairs)


function part1(){
    let correctPairs = []
    for (let pair_i = 0; pair_i < pairs.length; pair_i++) {
        let res = []
        res = comparePackets(pairs[pair_i][0], pairs[pair_i][1], res)
        for (let x = 0; x < res.length; x++) {
            if (res[x] == 0){
                break
            } else if(res[x] == 1) {
                correctPairs.push(pair_i+1) 
                break
            }
        }        
    }
    return correctPairs.reduce((partialSum, a) => partialSum+a, 0);
}

function part2(){
    let dividerPackets = [[[2]], [[6]]]
    pairs.push(dividerPackets)
    let sorted = [] //[...dividerPackets]

    for (let pair_i = 0; pair_i < pairs.length; pair_i++) {
        for (let leftright = 0; leftright < 2; leftright++) {

            let res = false
            loopCheckSpot: for (let spot = 0; spot < sorted.length; spot++) {
                res = checkSpot(pairs[pair_i][leftright], sorted[spot])
                if(res == true){
                    sorted.splice(spot, 0, pairs[pair_i][leftright]);
                    break loopCheckSpot
                }
            }
            if(sorted.length == 0 || res == false)
                sorted.push(pairs[pair_i][leftright])
        }
        
    }

    dividerPackets = [JSON.stringify(dividerPackets[0]), JSON.stringify(dividerPackets[1])]
    const decoderKey = sorted.reduce( (accumulator, currentValue, index) => {
        let val = JSON.stringify(currentValue)
        if(val == dividerPackets[0] || val == dividerPackets[1])
            return accumulator *(index+1)
        else 
            return accumulator
    }, 1);
    return decoderKey
}

function checkSpot(newPacket, packetToCheck){
    let res = comparePackets(newPacket, packetToCheck)
    for (let x = 0; x < res.length; x++) {
        if (res[x] == 0)
            return false    // pair incorrect
        else if(res[x] == 1) 
            return true // pair correct
    }
}

function comparePackets(le, ri, res = []){
    let [left, right] = [JSON.parse(JSON.stringify(le)), JSON.parse(JSON.stringify(ri))]
    let [leftType, rightType] = [typeof left, typeof right]

    if(leftType == 'number' && left != undefined)
        left = [left]        
    if(rightType == 'number' && right != undefined)
        right = [right]

    switch (true) {
        case (right == undefined && left != undefined):
            res.push(0)  
        case (right != undefined && left == undefined):
            res.push(1) 
        case (right == undefined || left == undefined):
            break;

        case (right.at(0) == undefined && left.at(0) != undefined):
            res.push(0)  
        case (right.at(0) != undefined && left.at(0) == undefined):
            res.push(1)  
        case (right.at(0) == undefined || left.at(0) == undefined):
            break;

        case (typeof left.at(0) == 'object' || typeof right.at(0) == 'object'):
            res = comparePackets(left.at(0), right.at(0), res)
            break;

        case (right.at(0) < left.at(0)):
            res.push(0)  
        case (right.at(0) > left.at(0)):
            res.push(1) 
    }
    
    if(left.length > 0 && right.length > 0){
        left.shift()
        right.shift()
        res = comparePackets(left, right, res)
    }

    return res
}





const t1 = performance.now();
console.log(utils.currentDateTime(), t1)
console.log(`${utils.rgb(0,255,0)}Day ${day}. \t Part1: ${part1()} \t Part2: ${part2()} \t\t ${Math.round((t1 - t0))} ms ${utils.reset()} `);
// Day 13.          Part1: 6101    Part2: 21909              6 ms 
