const fs = require('fs');
let utils = require('./utils.js');

let input_data = utils.readFileToArray('./data/day8.txt');
//console.log(input_data)


class Part1 {
	constructor() {
        this.visibleTrees = 0;
        this.printMatrix = ''
		this.iterate();
	}
	iterate() {
        let matrix = input_data
        let invertedMatrix = invertMatrix(matrix)
        for (let i = 0; i < input_data.length; i++) {
            for (let y = 0; y < input_data[i].length; y++) {
                if(
                    i == input_data.length-1 || i == 0 ||
                    y == input_data[i].length-1 || y == 0 ||
                    this.checkIfVisible(matrix[i], y) || 
                    this.checkIfVisible(invertedMatrix[y], i)
                    ){
                        this.visibleTrees += 1
                        this.printMatrix = this.printMatrix + `\x1b[42m${input_data[i][y]}\x1b[0m`
                }
                else {
                    this.printMatrix = this.printMatrix + `\x1b[41m${input_data[i][y]}\x1b[0m`
                }
                if(y == input_data.length-1){
                    this.printMatrix = this.printMatrix + `\n\r`
                }
                    
            }
        }
        
	}
    checkIfVisible(row, pos){
        let visible = {left: true, right: true}
        for (let i = 0; i < row.length; i++) {
            if(i < pos && Number(row[i]) >= Number(row[pos]))
                visible.left = false
            else if(i > pos && Number(row[i]) >= Number(row[pos]))
                visible.right = false
        }
        let vis = false
        if(visible.left == true || visible.right == true)
            vis = true
        return vis
    }
}

  

class Part2 {
	constructor() {
        this.topScore = 0;
        this.scores = []
		this.iterate();
	}

    
	iterate() {
        let matrix = input_data
        let invertedMatrix = invertMatrix(matrix)

        for (let i = 0; i < matrix.length; i++) {
            for (let y = 0; y < matrix[i].length; y++) {
                let score = 1
                let row = [...matrix[i]]
                let col = [...invertedMatrix[y]]
                let consideredTree = matrix[i][y]
                let directions = {
                    left: row.slice(0, y).reverse(), 
                    right: row.slice(y+1), 
                    up: col.slice(0, i).reverse(), 
                    down: col.slice(i+1)
                }
                
                for (const [direction, directionArray] of Object.entries(directions)) score *= Number(this.getScore(directionArray, consideredTree))
                if(score>this.topScore) this.topScore=score

                if(!this.scores[i])
                    this.scores.push([])
                if(!this.scores[i][y])
                    this.scores[i].push([])
                this.scores[i][y] = score
            }
        }
        
	}
    getScore(trees, treeHouse){
        for (let i = 0; i < trees.length; i++) {
            if(trees[i] >= treeHouse || i == trees.length-1)
                return i +1 
        }
        return 0
    }
    printMatrix(){
        let arr = this.scores
        let rbgScale = 255/(this.topScore-0)
        let fullString = ''
        for (let i = 0; i < arr.length; i++) {
            for (let y = 0; y < arr[i].length; y++) {
                let rgbGreen = Math.round( rbgScale * (arr[i][y]-0)) 
                if(arr[i][y].toString().length >1)
                    fullString += `\x1B[4m`
                fullString += `${utils.rgb(0,rgbGreen,0, 'bg')}${utils.rgb(rgbGreen,rgbGreen,rgbGreen, 'fg')}${arr[i][y]}\x1b[0m`
            }
            fullString += `\n\r`
            
        }
        return fullString
    }
    
}


function invertMatrix(matrix){
    let invertedMatrix = []
    for (let i = 0; i < matrix.length; i++) {
        for (let y = 0; y < matrix[i].length; y++) {
            if(!invertedMatrix[y])
                invertedMatrix.push([])
            invertedMatrix[y].push(matrix[i][y])
        }
    }
    return invertedMatrix
}






let p1 = new Part1(); // 1787 
let p2 = new Part2(); // 440640

console.log('\r\n p1:'); console.log(p1.printMatrix)
console.log('\r\n p2:'); console.log(p2.printMatrix())


p2.printMatrix()
console.log(`Part1: ${p1.visibleTrees} \t Part2: ${p2.topScore} `);
// Part1: 1787 --- Part2: 440640




