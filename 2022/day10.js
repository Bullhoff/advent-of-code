const fs = require('fs');
let utils = require('./utils.js');

let input_data = utils.readFileToArray('./data/day10.txt');
//console.log(input_data)

class Part1n2 {
	constructor() {
        this.signal = 1
        this.signalSum = 0
        this.col = []
        this.cycle = 0
        this.CRT = []
        this.CRTString = ''
		this.iterate();
        
	}

    
	iterate() {
        let commands = input_data
        let currentRow = []
        for (let i = 0; i < commands.length; i++) {
            if(commands[i] == 'noop')
                currentRow = this.cycleIt(1, currentRow)
            else {
                if(commands[i].split(' ')[0] == 'addx'){
                    currentRow = this.cycleIt(2, currentRow)
                    this.signal += Number(commands[i].split(' ')[1])
                }
            }
        }
        
        this.signalSum = this.calcSignalSum()
        currentRow = this.addCRTRow(currentRow)

        utils.writeFile({col:this.col,signalSum: this.signalSum}, 'day10.1')
        utils.writeFile(this.CRTString, 'day10.2')

        //console.table(this.col)
        //console.table(this.CRT)
    }
    
    cycleIt(cycles, currentRow){
        for (let i = 0; i < cycles; i++) {
            this.cycle += 1
            this.saveIfAtCheckpoint(this.cycle)
            currentRow = this.addCRTRow(currentRow)

            if(Math.abs(this.signal-currentRow.length) <= 1)
                currentRow.push('#')
            else 
                currentRow.push('.')
            
        }
        return currentRow
        
    }
    saveIfAtCheckpoint(cycle){
        let cycleCheckpoints = [20, 60, 100, 140, 180, 220]
        for (let i = 0; i < cycleCheckpoints.length; i++) {
            if(cycle == cycleCheckpoints[i]){
                this.col.push({cycle: this.cycle,signal: this.signal})
            }
        }
    }
    calcSignalSum(){
        let sum = 0
        for (let i = 0; i < this.col.length; i++) {
            this.col[i]['res'] = this.col[i].cycle * this.col[i].signal
            sum += this.col[i].cycle * this.col[i].signal
        }
        return sum
    }    
    addCRTRow(currentRow){
        if(currentRow && currentRow.length == 40){
            this.CRT.push(currentRow.join(''))
            this.CRTString += '\n' + currentRow.join('')
            currentRow = []
        }
        return currentRow
    }
    
}


let p1n2 = new Part1n2();         

console.log(`Part1: ${p1n2.signalSum} --- Part2: ${p1n2.CRTString} `);
// Part1: 13920 --- Part2: EGLHBLFJ



