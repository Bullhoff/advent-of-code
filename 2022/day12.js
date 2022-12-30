

let utils = require('./utils.js');
const t0 = performance.now();
console.log(utils.currentDateTime(), t0)
let day = 12
let input_data = utils.readFileToArray(`./data/day${day}.txt`, 1);   /// \n\s*\n/   // /\r\n\s*\r\n/
//console.table(input_data)


let startPos,goalPos
let grid = input_data.map((row, y) => row.split('').map((col, x) => { 
        if(col == 'S')
            startPos = {x:x, y:y}
        else if (col == 'E')
            goalPos = {x:x, y:y}
        return col.replace('S','a').replace('E','z').charCodeAt(0)-97
    }
));
let toPrint = []
//let printable = grid.map(x => x.join(''))
//console.log(printable)




class findPath {
	constructor(pos = [0, 0]) {
        this.pos = pos
    }
    getCost(nodeA, nodeB){
        return Math.abs(nodeA.x - nodeB.x)+Math.abs(nodeA.y - nodeB.y);
    }
    setObj(direction, newPos, parent, parentDistanceTravelled = 0, goalPosition){
        //console.log(newPos, goalPosition)
        let [g, h] = [parentDistanceTravelled+1, this.getCost(newPos, goalPosition)]//[getCost(newPos, startPos), getCost(newPos, goalPos)]
        return {
            direction:direction, 
            letter:input_data[newPos.y][newPos.x], 
            height: grid[newPos.y][newPos.x], 
            position: newPos, 
            distanceTravelled: g, 
            distanceLeft: h, 
            distanceTotal: g+h, 
            parent: parent
        }
    }

    checkBorders(pos, distanceTravelled, goalPosition){
        let bordering = []
        if(pos.x != grid[0].length-1)
            bordering.push(this.setObj('r', {x: pos.x+1, y: pos.y}, {x: pos.x, y: pos.y}, distanceTravelled, goalPosition))
        if(pos.x != 0)
            bordering.push(this.setObj('l', {x: pos.x-1, y: pos.y}, {x: pos.x, y: pos.y}, distanceTravelled, goalPosition))
        if(pos.y != grid.length-1)
            bordering.push(this.setObj('d', {x: pos.x, y: pos.y+1}, {x: pos.x, y: pos.y}, distanceTravelled, goalPosition))
        if(pos.y != 0)
            bordering.push(this.setObj('u', {x: pos.x, y: pos.y-1}, {x: pos.x, y: pos.y}, distanceTravelled, goalPosition))
        return bordering        //Object.values(bordering).sort((a,b)=>b['height']-a['height'])
    }
    checkHeightRequirement(fromHeight, toHeight){
        if(toHeight <= fromHeight+1)
            return true
        else 
            return false
    }
    getClosestPath(processed){
        let path = []
        let currentPos = goalPos
        let goalIndex = processed.findIndex(obj => JSON.stringify(obj.position) == JSON.stringify(goalPos))
        path.push(processed[goalIndex])
        while(true){
            let processedIndex = processed.findIndex(obj => JSON.stringify(obj.position) == JSON.stringify(path.at(-1).parent))
            path.push(processed[processedIndex])
            currentPos = processed[processedIndex]
            if(JSON.stringify(currentPos.position) == JSON.stringify(startPos))
                break
        }
        return path
        
    }
    
    
}



let [loopCount, winnerFound, path] = [0, false, []]
let [que,processed] = [[],[]]

let p = new findPath();

let p1 = part1()
function part1(){
    let pos = p.setObj('S', startPos,'-',0 ,goalPos)
    while(!winnerFound){
        loopCount += 1
        let bordering = p.checkBorders(pos.position, pos.distanceTravelled, goalPos)

        bordering.forEach(newObj => {
            let queIndex = que.findIndex(obj => JSON.stringify(obj.position) == JSON.stringify(newObj.position))
            let processedIndex = processed.findIndex(obj => JSON.stringify(obj.position) == JSON.stringify(newObj.position))
            let heightReq = p.checkHeightRequirement(pos.height, newObj.height)
            if(queIndex == -1 && processedIndex == -1 && heightReq)
                que.push(newObj)
            else if(queIndex != -1 && (que[queIndex].distanceTravelled > newObj.distanceTravelled) && heightReq)
                que[queIndex] = newObj
            //else if(processedIndex != -1 && (processed[processedIndex].distanceTravelled > newObj.distanceTravelled) && heightReq)
            //    processed[processedIndex] = newObj
            
        })
        if(que.length < 1)
            break

        pos = Object.values(que).sort((a,b)=>a['distanceTotal']-b['distanceTotal'])
        pos = pos.filter(val => val.distanceTotal == pos[0].distanceTotal).sort((a,b)=>a['distanceLeft']-b['distanceLeft'])[0];
        processed.push(pos)

        let posQueIndex = que.findIndex(obj => JSON.stringify(obj.position) == JSON.stringify(pos.position))
        que.splice(posQueIndex, 1)
        //if(JSON.stringify(pos.position) == JSON.stringify(goalPos))        winnerFound = true
        //if(winnerFound)         break
    }
    path = p.getClosestPath(processed)
    return {path: path.map(x => x.position), processed: processed}
}



let p2 = part2()
function part2(){
    return -11+2-1
    let goalPositions = processed.filter(val => val.height == 1)
    console.log(goalPositions)
    startPos = goalPos
    let pos = p.setObj('S', startPos,'-',0 ,goalPositions[0])
    let bordering = []

    que = []
    processed = []
    while(!winnerFound){
        loopCount += 1
        
        for (let i = 0; i < goalPositions.length; i++) {
            //bordering = p.checkBorders(pos.position, pos.distanceTravelled, goalPositions[i])
            console.log(012345, goalPositions[i])
            if(!goalPositions[i]) continue
            bordering = p.checkBorders(goalPositions[i], pos.distanceTravelled, pos.position)
            bordering.forEach(goal => {
                if(goal.height == 0){
                    console.log('goal', goal)
                    pos = p.setObj('S', startPos,'-',0 ,goalPositions[i])
                }
            })
            //bordering = p.checkBorders(pos.position, pos.distanceTravelled, goalPositions[i])
            
        }
        //goalPos = Object.values(bordering).sort((a,b)=>a['distanceTotal']-b['distanceTotal'])
        console.log(' pos:',pos, ' goalPos:',goalPos[0])
        break

        bordering.forEach(newObj => {
            let queIndex = que.findIndex(obj => JSON.stringify(obj.position) == JSON.stringify(newObj.position))
            let processedIndex = processed.findIndex(obj => JSON.stringify(obj.position) == JSON.stringify(newObj.position))
            let heightReq = p.checkHeightRequirement(pos.height, newObj.height)
            if(queIndex == -1 && processedIndex == -1 && heightReq)
                que.push(newObj)
            else if(queIndex != -1 && (que[queIndex].distanceTravelled > newObj.distanceTravelled) && heightReq)
                que[queIndex] = newObj
            
        })
        if(que.length < 1)
            break

        pos = Object.values(que).sort((a,b)=>a['distanceTotal']-b['distanceTotal'])
        pos = pos.filter(val => val.distanceTotal == pos[0].distanceTotal).sort((a,b)=>a['distanceLeft']-b['distanceLeft'])[0];
        processed.push(pos)

        let posQueIndex = que.findIndex(obj => JSON.stringify(obj.position) == JSON.stringify(pos.position))
        que.splice(posQueIndex, 1)
        //if(JSON.stringify(pos.position) == JSON.stringify(goalPos))        winnerFound = true        //if(winnerFound)         break
    }
    //path = p.getClosestPath(processed)
    //console.log('PathLenght:' + path.length, 'loopCount:'+loopCount)
}


console.log('Part 1:')
console.log(printMatrix(p1.path, grid))

const t1 = performance.now();
console.log(utils.currentDateTime(), t1)

console.log(`${utils.rgb(0,255,0)}Day ${day}. \t Part1: ${path.length-1} \t Part2: ${p2+path.length} \t\t ${Math.round((t1 - t0))} ms ${utils.reset()} `);
// Day 12.          Part1: 330    Part2: 321              4386 ms 




function printMatrix(path, arr){
    let rbgScale = 255/25
    let fullString = ''
    for (let i = 0; i < arr.length; i++) {
        for (let y = 0; y < arr[i].length; y++) {
            let rgbGreen = Math.round( rbgScale * (arr[i][y]-0)) 
            //if(arr[i][y].toString().length >1)
            //    fullString += `\x1B[4m`
            let inPath = false
            for (let x = 0; x < path.length; x++) {
                if(path[x].x == y && path[x].y == i)
                    inPath = true
            }
            if(JSON.stringify([path.at(-1).y, path.at(-1).x]) == JSON.stringify([i,y]))
                fullString += `\x1B[4m${utils.rgb(255,0,255, 'bg')}`
            else if(inPath)
                fullString += `\x1B[4m${utils.rgb(0,0,255, 'bg')}`
            else if(i == startPos[0] && y == startPos[1])
                fullString += `\x1B[4m${utils.rgb(255,rgbGreen,0, 'bg')}`
            else if(i == goalPos[0] && y == goalPos[1])
                fullString += `\x1B[4m${utils.rgb(255,rgbGreen,0, 'bg')}`
            else
                fullString += `${utils.rgb(0,rgbGreen,0, 'bg')}`


            if(JSON.stringify([i,y]) == JSON.stringify([30,55]))
                fullString += `${utils.rgb(255,0,0, 'bg')}`
            fullString += `${utils.rgb(rgbGreen,rgbGreen,rgbGreen, 'fg')}${utils.normalizeLength(arr[i][y],-2, ' ')}\x1b[0m`
            //fullString += `${utils.rgb(0,rgbGreen,0, 'bg')}${utils.rgb(rgbGreen,rgbGreen,rgbGreen, 'fg')}${utils.normalizeLength(arr[i][y],-2, ' ')}\x1b[0m`
        }
        fullString += `\n\r`
        
    }
    return fullString
}

