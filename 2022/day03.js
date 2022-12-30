const fs = require('fs');

function readFileToArray(filename) {
	const contents = fs.readFileSync(filename, 'utf-8');
	const arr = contents.split(/\r?\n/);
	return arr;
}

let input_data = readFileToArray('./data/day03.txt');
console.log(input_data)

function getPriority(item) {
	if (item == item.toUpperCase()) 
        return item.charCodeAt(0) - 38; 
	else 
        return priority = item.charCodeAt(0) - 96;
}




class Part1 {
	constructor() {
		this.prioritySum = 0;
		this.iterate();
	}

	iterate() {
		for (let i = 0; i < input_data.length; i++) {
			let compartmentItems = [input_data[i].slice(0, input_data[i].length / 2), input_data[i].slice(input_data[i].length / 2, input_data[i].length)];
			let priority = 0;

			for (let y = 0; y < compartmentItems[0].length; y++) {
				if (compartmentItems[1].includes(compartmentItems[0][y])) {
                    priority = getPriority(compartmentItems[0][y])
				}
			}

			this.prioritySum += priority;
		}
	}
}

class Part2 {
	constructor() {
		this.prioritySum = 0;
		this.iterate();
	}
	iterate() {
		for (let i = 0; i < input_data.length; i += 3) {
			let groupsRucksacks = input_data.slice(i, i + 3);
			groupsRucksacks = groupsRucksacks.sort(function (a, b) {return b.length - a.length;});
            let priority = 0

			for (let i = 0; i < groupsRucksacks[0].length; i++) {
				if (groupsRucksacks[1].includes(groupsRucksacks[0][i]) && groupsRucksacks[2].includes(groupsRucksacks[0][i])) {
                    priority = getPriority(groupsRucksacks[0][i])
				}
			}

            this.prioritySum += priority
		}
	}
}

let p1 = new Part1(); // 7446
let p2 = new Part2(); // 2646
console.log(`Part1: ${p1.prioritySum} --- Part2: ${p2.prioritySum} `);
// Part1: 7446 --- Part2: 2646
