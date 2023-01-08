const fs = require('fs');

function readFileToArray(filename) {
	const contents = fs.readFileSync(filename, 'utf-8');
	const arr = contents.split(/\r?\n/);
	return arr;
}

let input_data = readFileToArray('./data/day06.txt')[0];

const hasDuplicates = (arr) => arr.length !== new Set(arr).size;

function iterate(sequenceLength) {
	let marker = [];

	for (let i = 0; i < input_data.length; i++) {
		marker.push(input_data[i]);

		if (marker.length > sequenceLength) marker.shift();

		if (!hasDuplicates(marker) && marker.length == sequenceLength) return i + 1;
	}
}

console.log(`Part1: ${iterate(4)} --- Part2: ${iterate(14)} `);
// Part1: 1538 --- Part2: 2315
