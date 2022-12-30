
let utils = require('./utils.js');
let day = 11
let input_data = utils.readFileToArray(`./data/day${day}.txt`, 1, /\r\n\s*\r\n/);   /// \n\s*\n/

class Part1n2 {
	constructor() {
        this.part1 = this.iterate(20);
        this.part2 = this.iterate(10000, 1);
    }

	iterate(rounds, worryHandler = 3) {
        let monkeyArr = []
        const t0 = performance.now();

        let [round, thrownTo, output] = [0, 0, 0]
        while(round < rounds ){
            round += 1
            let reducedTest = input_data.reduce((a, b) => a * b.split('\r\n')[3].split(' ').at(-1), 1);
            for (let monkey = 0; monkey < input_data.length; monkey++) {                
                
                if(!monkeyArr[monkey])
                    monkeyArr[monkey] = this.createMonkey(monkey)

                while(monkeyArr[monkey].items[0] != undefined){
                    if(!monkeyArr[monkey]['itemsPassed'])
                        monkeyArr[monkey]['itemsPassed'] = 0
                    monkeyArr[monkey]['itemsPassed'] += 1

                    let itemValue = this.operation(monkeyArr[monkey].operation, monkeyArr[monkey].items[0])
                    itemValue = Math.floor(itemValue/worryHandler) % reducedTest
                    
                    let res = this.test(monkeyArr[monkey].test, itemValue)
                    if(res)
                        thrownTo = monkeyArr[monkey].ifTrue.at(-1)
                    else
                        thrownTo = monkeyArr[monkey].ifFalse.at(-1)
                    
                    if(!monkeyArr[thrownTo])
                        monkeyArr[thrownTo] = this.createMonkey(thrownTo)

                    monkeyArr[thrownTo].items.push(itemValue)
                    monkeyArr[monkey].items.shift()

                    //utils.logger(`round:${round} Monkey:${monkey} itemsleft:${monkeyArr[monkey].items.length} \t itemValue: ${utils.normalizeLength(itemValue,8)} \t testRes: ${utils.normalizeLength(res, 5)} \t itemsPassed:${utils.normalizeLength(monkeyArr[monkey]['itemsPassed'],4)} \t ${utils.normalizeLength('Throws ' + itemValue + ' to ' + thrownTo, 20)} \t ${monkeyArr[monkey].items} --- ${monkeyArr[thrownTo].items} \t `)
                    
                }
            }
            /* for (let i = 0; i < monkeyArr.length; i++) {
                utils.logger(`monkey:${i}, round:${round} \t itemsPassed:${monkeyArr[i].itemsPassed} \t items:${monkeyArr[i].items}`)
            } */
        }
        /* for (let i = 0; i < monkeyArr.length; i++) {
            utils.logger(`monkey:${i} \t itemsPassed:${monkeyArr[i].itemsPassed} \t items:${monkeyArr[i].items}`)
        } */
        let monkeyBusinessLevel = Number(utils.sortArrayObject(monkeyArr, 'itemsPassed').at(-1).itemsPassed)*Number(utils.sortArrayObject(monkeyArr, 'itemsPassed').at(-2).itemsPassed)
        const t1 = performance.now();
        utils.logger(`rounds:${rounds} \t worryHandler:${worryHandler} \t monkeyBusinessLevel:${monkeyBusinessLevel} \t ${Math.round((t1 - t0))} ms \n`)
        return monkeyBusinessLevel
    }
    test(te, itemValue){
        //let res = [itemValue/te.at(-1), Math.round(itemValue/te.at(-1))]
        //if(res[0] == res[1])
        let res = itemValue % te.at(-1)
        if(res == 0n)
            return true
        else 
            return false
    }
    operation(op, item){
        let numbers = []
        for (let i = 0; i < [2, 4].length; i++) {
            if(op.at([2, 4][i]) == 'old')
                numbers.push(item)
            else 
                numbers.push(op[[2, 4][i]])
        }
        return utils.operators[op[3]](Number(numbers[0]), Number(numbers[1]))
    }
    createMonkey(monkey){
        let monkeyData = input_data[monkey].split(/\r?\n/)
        return  {
            monkey : monkeyData[0].split(':')[0].split(' '),
            items : monkeyData[1].split(': ')[1].split(', '),
            operation : monkeyData[2].split(': ')[1].split(' '), 
            test : monkeyData[3].split(': ')[1].split(' '),
            ifTrue : monkeyData[4].split(': ')[1].split(' '),
            ifFalse : monkeyData[5].split(': ')[1].split(' ')
        }
    }
    throw(){

    }
    
}



const t0 = performance.now();
let p1n2 = new Part1n2();
const t1 = performance.now();

console.log(`${utils.rgb(0,255,0)}Day ${day}. \t Part1: ${p1n2.part1} \t Part2: ${p1n2.part2} \t\t ${Math.round((t1 - t0))} ms ${utils.reset()} `);
// Day 11.          Part1: 54036    Part2: 13237873355              191 ms 