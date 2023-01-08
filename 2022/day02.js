const fs = require('fs');

function readFileToArray(filename) {
	const contents = fs.readFileSync(filename, 'utf-8');
	const arr = contents.split(/\r?\n/);
	return arr;
}

let input_data = readFileToArray('./data/day02.txt');
console.log(input_data);

function shapeNumber(shape) {
	if (shape == 'A' || shape == 'X') return 1; //'Rock';
	else if (shape == 'B' || shape == 'Y') return 2; // 'Paper';
	else if (shape == 'C' || shape == 'Z') return 3; // 'Scissors';
}
function winPoint(moves) {
	if ((moves[0] == 1 && moves[1] == 2) || (moves[0] == 2 && moves[1] == 3) || (moves[0] == 3 && moves[1] == 1)) return 6;
	else if (moves[0] == moves[1]) return 3;
	else return 0;
}

// A for Rock, B for Paper, and C for Scissors.
// X for Rock, Y for Paper, and Z for Scissors.
// Rock, 1   , Paper, 2   ,     Scissors, 3
// 0,lost   3,draw   6,won
class Part1 {
	constructor(myScore) {
		this.myScore = 0;
		this.iterate();
	}

	iterate() {
		for (let i = 0; i < input_data.length; i++) {
			let roundMoves = input_data[i].split(' ');
			roundMoves[0] = shapeNumber(roundMoves[0]);
			roundMoves[1] = shapeNumber(roundMoves[1]);
			this.myScore += winPoint(roundMoves);
			this.myScore += roundMoves[1];
		}
	}
}

// X means you need to lose, Y means you need to end the round in a draw, and Z means you need to win.
// A for Rock, B for Paper, and C for Scissors.
class Part2 {
	constructor(myScore) {
		this.myScore = 0;
		this.iterate();
	}

	iterate() {
		for (let i = 0; i < input_data.length; i++) {
			let roundMoves = input_data[i].split(' ');
			roundMoves[0] = shapeNumber(roundMoves[0]);
			roundMoves[1] = this.adaptMyMove(roundMoves);
			this.myScore += winPoint(roundMoves);
			this.myScore += roundMoves[1];
		}
	}

	adaptMyMove(moves) {
		let myMove = 0;

		if (moves[1] == 'X') myMove = moves[0] - 1;
		else if (moves[1] == 'Y') myMove = moves[0];
		else if (moves[1] == 'Z') myMove = moves[0] + 1;

		if (myMove < 1) myMove = 3;
		else if (myMove > 3) myMove = 1;
		return myMove;
	}
}

let p1 = new Part1();
let p2 = new Part2();
console.log(`Part1: ${p1.myScore} --- Part2: ${p2.myScore} `);
// Part1: 10624 --- Part2: 14060
