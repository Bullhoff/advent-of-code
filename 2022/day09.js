const fs = require('fs');
let utils = require('./utils.js');
let day = 'day09'
let input_data = utils.readFileToArray(`./data/${day}.txt`, 2);
console.log(input_data)



class Part1 {
	constructor() {
        this.positionsVisited = 1;
		this.iterate();
	}

    
	iterate() {
        let hMoves = input_data
        
        let hLocation = [0,0]
        let tLocation = [0,0]

        
        let [movesSum, moves, plane] = this.setStuff(hMoves)
        let tPositionsAll = []
        tPositionsAll.push(`${tLocation[0]}, ${tLocation[1]}`)
        for (let i = 0; i < moves.length; i++) {
            hLocation = [
                Number(hLocation[0]) + Number(moves[i][0]), 
                Number(hLocation[1]) + Number(moves[i][1])
            ]
            let diff = [
                Math.abs(hLocation[0]-tLocation[0]), 
                Math.abs(hLocation[1]-tLocation[1])
            ]

            for (let y = 0; y < Math.abs(moves[i][0]+moves[i][1]); y++) {
                diff[0] = Math.abs(hLocation[0]-tLocation[0])
                diff[1] = Math.abs(hLocation[1]-tLocation[1])
                if((diff[0] <= 1 && diff[1] <= 1 && Math.abs(diff[0]+diff[1]) <= 2)){
                    break
                }
                if(!isNaN(diff[0]/diff[0]) && !isNaN(diff[1]/diff[1])){ 
                    tLocation[0] += Math.sign(hLocation[0]-tLocation[0])
                    tLocation[1] += Math.sign(hLocation[1]-tLocation[1])
                }

                else if(!isNaN(hLocation[0]/tLocation[0]) && !isNaN(moves[i][0]/moves[i][0])){
                    tLocation[0] += Math.sign(moves[i][0])
                }
                else if(!isNaN(hLocation[1]/tLocation[1]) && !isNaN(moves[i][1]/moves[i][1])){
                    tLocation[1] += Math.sign(moves[i][1])
                }
                tPositionsAll.push(`${tLocation[0]}, ${tLocation[1]}`)
                
                    
            }
            
        }
        const unique = Array.from(new Set(tPositionsAll)) 
        this.positionsVisited = unique.length
	}
    addPositionVisit(plane, tLocation){
        console.log(`${'addPositionVisit(plane, tLocation)'} \t ${'plane'} \t ${tLocation}`)
        if(plane[tLocation[0]][tLocation[1]] == 0)
            this.positionsVisited += 1
        plane[tLocation[0]][tLocation[1]] += 1
    }
    setStuff(hMoves){
        let moves = []
        for (let i = 0; i < hMoves.length; i++) {
            moves.push(this.translateMove(hMoves[i]))
        }
        let movesSum = [0, 0]
        let largestPos = [0, 0]
        let currentPos = [0, 0]
        for (let i = 0; i < moves.length; i++) {
            currentPos[0] += moves[i][0] 
            currentPos[1] += moves[i][1] 
            if(currentPos[0] > largestPos[0])
                largestPos[0] = currentPos[0]
            if(currentPos[1] > largestPos[1])
                largestPos[1] = currentPos[1]
            //if(moves[i][0] > 0)
             //   movesSum[0] += moves[i][0] 
            //if(moves[i][1] > 0)
            //    movesSum[1] += moves[i][1]
            //console.log(largestPos, moves[i])
        }
        let plane = []
        for (let i = 0; i < largestPos[0]; i++) {
            plane.push([0])
            for (let y = 0; y < largestPos[1]; y++) {
                plane[i].push(0);
            }
        }
        return [largestPos, moves, plane]
    }
    move(plane, hLocation, [xMove, yMove]){
        plane = this.addLayer(plane, hLocation, steps)
        //console.log(`*move* \t ${plane} \t ${hLocation} \t ${hLocation[0]} \t ${direction} ${steps} \t .${plane[hLocation[0]]} \t .${plane}`)
        
        for (let i = 0; i < plane.length; i++) {
            plane = this.addLayer(plane, hLocation, xMove)
            for (let y = 0; y < plane[i].length; y++) {

            }
        }
        return plane
    }
    addLayer(plane, currentLocation, steps){
        for (let i = 0; i < steps; i++) {
            console.log(i, currentLocation, steps, '\t' , plane)
            if(!plane[currentLocation[0]+i])
                plane.push(0)
        }
        return plane
    }


    translateMove(move){
        let direction = move.split(' ')[0]
        let steps = Number(move.split(' ')[1])
        if(direction=='R')
            return [steps, 0]
        else if(direction=='L')
            return [-steps, 0]
        else if(direction=='U')
            return [0, steps]
        else if(direction=='D')
            return [0, -steps]
    }
    
    
}



class Part2 {
	constructor() {
        
        this.moves = this.translateMoves(input_data)
        this.maxPos = this.getMaxCoordinates() //{r:0, l:0, u:0, d:0}
        this.positions = Array(10).fill({y: Math.abs(this.maxPos.d), x: Math.abs(this.maxPos.l)})   //[]
        this.startPos = Array(10).fill({y: Math.abs(this.maxPos.d), x: Math.abs(this.maxPos.l)})    //{}
        this.posHistory = []    //this.posHistory.push(JSON.parse(JSON.stringify(this.positions)))
        this.planeSize = {y:this.maxPos.u-this.maxPos.d +1, x:this.maxPos.r-this.maxPos.l +1}
        this.plane = this.createPlane()
        //this.positionsVisited = new Set(this.posHistory.map(i => JSON.stringify(i[9])));
		this.iterate();
	}
    getVisitedPositions(){
        return new Set(this.posHistory.map(i => JSON.stringify(i[9])))
    }
	iterate() {
        console.log(utils.rgb(0,255,0) + 'iterate() Part 2' + utils.reset())

        for (let i = 0; i < this.moves.length; i++) {
            let movesAmout = Math.abs(Math.abs(this.moves[i].y)+Math.abs(this.moves[i].x))
            this.positions[0].x += this.moves[i].x
            this.positions[0].y += this.moves[i].y

            for (let y = 0; y < movesAmout; y++) {

                for (let x = 1; x < this.positions.length; x++) {
                    this.positions[x] = this.move(this.positions[x-1], this.positions[x], x)
                }
                this.posHistory.push(JSON.parse(JSON.stringify(this.positions)))
            }
        }
	}
    move(he, te, knot){
        let h = JSON.parse(JSON.stringify(he))
        let t = JSON.parse(JSON.stringify(te))

        let diff = {
            y: h.y-t.y,//Math.abs(h.y-t.y), 
            x: h.x-t.x//Math.abs(h.x-t.x)
        }
        let msg = `[tx: c ty: ${t.y}]   diffx:${diff.x} diffy:${diff.y}` // h:${JSON.stringify(h)}\t  t:${JSON.stringify(t)}
        
        if(Math.abs(diff.y) > 1 || Math.abs(diff.x) > 1){
            t.y = t.y + Math.sign(diff.y)
            t.x = t.x + Math.sign(diff.x)
        }
            
        //if(knot == 9)
            this.plane[t.y][t.x] = knot

        return t
    }
    getMaxCoordinates(){
        let currentPos = {x: 0, y:0}
        let max = {r:0, l:0, u:0, d:0}
        for (let i = 0; i < this.moves.length; i++) {
            currentPos.y += this.moves[i].y 
            currentPos.x += this.moves[i].x 
            if(currentPos.x > max.r)
                max.r = currentPos.x
            if(currentPos.x < max.l)
                max.l = currentPos.x
            if(currentPos.y > max.u)
                max.u = currentPos.y
            if(currentPos.y < max.d)
                max.d = currentPos.y
        }
        return max
    }
    createPlane(){
        let plane = []
        for (let i = 0; i < this.planeSize.y; i++) {
            plane.push([])
            for (let y = 0; y < this.planeSize.x; y++) {
                plane[i].push('');
            }
        }
        return plane
    }

    translateMoves(movesRaw){
        let moves = []
        for (let i = 0; i < movesRaw.length; i++) {
            let direction = movesRaw[i].split(' ')[0]
            let steps = parseInt(movesRaw[i].split(' ')[1]) //Number(move.split(' ')[1])
            if(direction=='R')
                moves.push({x: steps, y: 0})
            else if(direction=='L')
                moves.push({x: -steps, y: 0})
            else if(direction=='D')
                moves.push({x: 0, y: -steps})
            else if(direction=='U')
                moves.push({x: 0, y: steps})
        }
        return moves
    }
    
    
}


let p1 = new Part1(); // 6498 
let p2 = new Part2(); // 2531 



console.log(`Part1: ${p1.positionsVisited} --- Part2: ${p2.getVisitedPositions().size} `);
// Part1: 6498 --- Part2: 2531


let info = {
    planeSize: {y: p2.plane.length, x: p2.plane[0].length},
    total: p2.plane.length*p2.plane[0].length
}

let columns = ''//= info.planeSize.x.map(i => `1fr`).join(' ')
for (let i = 0; i < info.planeSize.x; i++) {
    columns += ' 1fr'
}
let style = `
    body{
        background-color:black;
        color:white;
    }
    div{
        color:white;
    }
    .floater{
        position: fixed;
        opacity: 0.8;
        left: 10%;
        top: 1%;
    }
    #container{
        display: grid ;
    }
    #container div{
        font-size: 0.1ch;
    }

    .day9 {
        grid-template-columns: ${columns} ;
    }
    .day9 div input{
        background-color: bisque;
        width: 2ch;
    }
    .day9 input[type=number] {
        background-color: bisque;
    }
    .grid-item{
        height:1ch;
        width:1ch;
        padding: 0% 0% 0% 0%;
        border:0.1px dotted #4c1074;
    }`

let script = `
    const posHistory = ${utils.replaceAll(JSON.stringify(p2.posHistory), "\"", "")}
    const startPos = ${utils.replaceAll(JSON.stringify(p2.startPos[0]), "\"", "")}
    var ids = []
    function makeGrid(rows, cols) {
        rows = ${info.planeSize.y}
        cols = ${info.planeSize.x}
        /* document.getElementById("test").innerHTML = "makeGrid..." */
        const container = document.getElementById("container");
        container.style.setProperty('--grid-rows', rows);
        container.style.setProperty('--grid-cols', cols);
        for (y = 0; y < rows; y++) {
            for (x = 0; x < cols; x++) {
                let cell = document.createElement("div");
                ids.push(x+","+y)
                cell.setAttribute("id", x+","+y);
                container.appendChild(cell).className = "grid-item";
            }    
        }
        fillGrid(0)
    };
    let currentStep=0
    function fillGrid(step) {
        if(step == 'end')
            currentStep=posHistory.length-1
        else if(step == 'start')
            currentStep=0
        else
            currentStep+=step
        clearGrid()

        let positions = posHistory.at(currentStep)
        document.getElementById("positions").innerHTML = currentStep + ' ==> ' + JSON.stringify(positions)
        for (i = 0; i < positions.length; i++) {
            let cell = document.getElementById(positions[i].x+","+positions[i].y);
            if(i == 0)
                cell.style.backgroundColor = "red";
            else if(i== 9)
                cell.style.backgroundColor = "lightgreen";
            else
                cell.style.backgroundColor = "yellow";
              
        }
    };
        function clearGrid() {
            var elements = document.getElementsByClassName('grid-item');
            for (var i = 0; i < elements.length; i++) {
                elements[i].style.backgroundColor = "";
            }
            let cell = document.getElementById(startPos.x + ',' + startPos.y);
            cell.style.backgroundColor = "gray";
        };
        function zoomer(e){
            for (let i = 0; i < ids.length; i++) {
                document.getElementById(ids[i]).style.width = e + 'ch';
                document.getElementById(ids[i]).style.height = e + 'ch';                
            }
        }
`

let body = `
    <div class="day9" id="container">
        <div id="floatertest" class="floater">
            <div id="positions">positions</div>
            <div id="test"></div>
            <br>
            <input type="number" class="size" id="size" name="size" value=1 onchange="zoomer(this.value)" style="width: 5ch;"/>
            <button onclick="fillGrid('start')">start</button>
            <button onclick="fillGrid(-100)">-100</button>
            <button onclick="fillGrid(-10)">-10</button>
            <button onclick="fillGrid(-1)">-1</button>
            <button onclick="fillGrid(1)">+1</button>
            <button onclick="fillGrid(10)">+10</button>
            <button onclick="fillGrid(100)">+100</button>
            <button onclick="fillGrid('end')">end</button>
        </div>
    </div>`;


let fullhtml = utils.html({body:body, bodyTag: 'onload="makeGrid(404, 250)"', style:style, styleExternal:'', scriptExternal:'', script:script})
utils.writeFile(fullhtml, day+'_part2', '.html')


