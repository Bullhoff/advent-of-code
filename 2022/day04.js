const fs = require('fs');

function readFileToArray(filename) {
	const contents = fs.readFileSync(filename, 'utf-8');
	const arr = contents.split(/\r?\n/);
	return arr;
}

let input_data = readFileToArray('./data/day04.txt');
console.log(input_data)



class Part1 {
	constructor() {
		this.overlapping = 0;
		this.iterate();
	}

	iterate() {
		for (let i = 0; i < input_data.length; i++) {
            let elfAssignment = input_data[i].split(',')
            elfAssignment = [elfAssignment[0].split('-').map(Number), elfAssignment[1].split('-').map(Number)]
            if(
                (elfAssignment[0][0] <= elfAssignment[1][0] && elfAssignment[0][1] >= elfAssignment[1][1]) ||
                (elfAssignment[0][0] >= elfAssignment[1][0] && elfAssignment[0][1] <= elfAssignment[1][1])
                ){
                this.overlapping += 1
            }
                
		}
	}
}

class Part2 {
	constructor() {
		this.overlapping = 0;
		this.iterate();
	}
	iterate() {
        for (let i = 0; i < input_data.length; i++) {
            let elfAssignment = input_data[i].split(',')
            elfAssignment = [elfAssignment[0].split('-').map(Number), elfAssignment[1].split('-').map(Number)]
            if(
                (elfAssignment[0][0] < elfAssignment[1][0] && elfAssignment[0][1] < elfAssignment[1][0]) ||
                (elfAssignment[0][1] > elfAssignment[1][1] && elfAssignment[0][0] > elfAssignment[1][1]) 
                ){
            }
            else{
                this.overlapping += 1
            }
            
                
		}
	}
}

let p1 = new Part1(); // 466    
let p2 = new Part2(); // 865  
console.log(`Part1: ${p1.overlapping} --- Part2: ${p2.overlapping} `);
// Part1: 466 --- Part2: 865


