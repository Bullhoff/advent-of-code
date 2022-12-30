const fs = require('fs');

function readFileToArray(filename) {
	const contents = fs.readFileSync(filename, 'utf-8');
	const arr = contents.split(/\r?\n/);
	return arr;
}

let input_data = readFileToArray('./data/day5.txt');
//console.log(input_data)

function fixStacksArray(input_data){
    let input_stacks = input_data.slice(0, input_data.indexOf('', 0))
    let stacksCount = Number(input_stacks.at(-1).split('   ').at(-1))
    
    for (let i = 0; i < input_stacks.length; i++) {
        let tempArr = []
        for (let y = 0; y < stacksCount; y++) {
            tempArr.push(input_stacks[i].slice(0+4*y, 3+4*y))
        }
        input_stacks[i] = tempArr
    }
    return input_stacks
}

function fixRearrangementsArray(){
    let input_rearrangements = input_data.slice(input_data.indexOf('', 0)+1)
    for (let i = 0; i < input_rearrangements.length; i++) {
        let tempArr = []
        tempArr.push(input_rearrangements[i].split(' ')[1]) // Stacks amount to move
        tempArr.push(input_rearrangements[i].split(' ')[3]) // From
        tempArr.push(input_rearrangements[i].split(' ')[5]) // To
        input_rearrangements[i] = tempArr
    }
    return input_rearrangements
}


function addRow(toStackIndex, crateToMove, columnsCount){
    let tempArr = []
    for (let i = 0; i < columnsCount; i++) {
        if(i == toStackIndex)
            tempArr.push(crateToMove)
        else 
            tempArr.push('   ')

    }
    return tempArr
}
function getTopItem(stackIndex, stacks){
    let rowNr = -1
    let crateToMove = '   '
    do {
        rowNr += 1
        crateToMove = stacks[rowNr][stackIndex]
    } while (crateToMove == '   ');

    return [rowNr, crateToMove]
}
function getAllTopCrates(columnsCount, stacks){
    let message = ''
    for (let i = 0; i < columnsCount; i++) {
        message += getTopItem(i, stacks)[1].slice(1,2)
    }

    return message
}




class Part1 {
	constructor() {
		this.stacks = fixStacksArray(input_data)
        this.rearrangements = fixRearrangementsArray(input_data)
		this.iterate();
        this.topCrates = getAllTopCrates(this.stacks[0].length, this.stacks);
	}

	iterate() {
        for (let i = 0; i < this.rearrangements.length; i++) {
            for (let y = 0; y < this.rearrangements[i][0]; y++) {
                
                let fromStackIndex = this.rearrangements[i][1]-1
                let [fromRowNr, crateToMove] = getTopItem(fromStackIndex, this.stacks)
                

                let toStackIndex = this.rearrangements[i][2]-1
                let [toRowNr, toCurrentTopCrate] = getTopItem(toStackIndex, this.stacks)
                toRowNr -= 1

                this.stacks[fromRowNr][fromStackIndex] = '   '

                if(toRowNr < 0){
                    let newRow = addRow(toStackIndex, crateToMove, this.stacks[0].length)
                    this.stacks.splice(0, 0, newRow)
                }
                else {
                    this.stacks[toRowNr][toStackIndex] = crateToMove
                }
                    
            }
        }
	}
}


class Part2 {
	constructor() {
		this.stacks = fixStacksArray(input_data)
        this.rearrangements = fixRearrangementsArray(input_data)
		this.iterate();
        this.topCrates = getAllTopCrates(this.stacks[0].length, this.stacks);
	}

	iterate() {
        for (let i = 0; i < this.rearrangements.length; i++) {

            let tempArr = []
            let movesAmount = this.rearrangements[i][0]
            let fromStackIndex = this.rearrangements[i][1]-1
            let [fromRowNr, crateToMove] = getTopItem(fromStackIndex, this.stacks)
            
            for (let y = 0; y < this.rearrangements[i][0] ; y++) {
                tempArr.push(this.stacks[fromRowNr+y][fromStackIndex])
                this.stacks[fromRowNr+y][fromStackIndex] = '   '
            }

            let toStackIndex = this.rearrangements[i][2]-1
            let [toRowNr, toCurrentTopCrate] = getTopItem(toStackIndex, this.stacks)
            toRowNr = toRowNr - 1

            tempArr.reverse();

            for (let y = 0; y < tempArr.length; y++) {
                if((toRowNr-y) < 0){
                    let newRow = addRow(toStackIndex, tempArr[y], this.stacks[0].length)
                    this.stacks.splice(0, 0, newRow) 
                }
                else {
                    this.stacks[toRowNr-y][toStackIndex] = tempArr[y]
                }

            }
        }
	}
}






let p1 = new Part1();   // JRVNHHCSJ    
let p2 = new Part2();   // GNFBSBJLH

console.log('\n Part 1 Table')
console.table(p1.stacks)

console.log('\n Part 2 Table')
console.table(p2.stacks)

console.log(`Part1: ${p1.topCrates} --- Part2: ${p2.topCrates} `);
// Part1: JRVNHHCSJ --- Part2: GNFBSBJLH




