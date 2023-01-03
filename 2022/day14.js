

let utils = require('./utils.js');
const t0 = performance.now();console.log(utils.currentDateTime(), t0);
let day = 14
let input_data = utils.readFileToArray(`./data/day${day}.txt`, 1, /\r\n/);   /// \n\s*\n/   // /\r\n\s*\r\n/



function parse(data, part2Expand = false){
    let limits = {r: 0, l: 999, d:0, u:0}
    let input = data.map(x => x.split('->').map(xx => {
        let val_x = parseInt(xx.split(',')[0])
        let val_y = parseInt(xx.split(',')[1])
        if(val_x > limits.r) limits.r = val_x
        if(val_x < limits.l) limits.l = val_x
        if(val_y > limits.d) limits.d = val_y
        return {x:val_x, y:val_y}
    }));
    if(part2Expand == true){
        limits.l = 500-limits.d-5
        limits.r = 500+limits.d+5
        limits.d = limits.d+2
        let border = [{x:limits.l, y: limits.d}, {x:limits.r, y: limits.d}]
        input.push(border)
    }
    return [input, limits]
}

function init(part2=false){
    let [input, limits] = parse(input_data, part2)
    let grid = Array.from({ length: limits.d+1 }, (v, y) => Array.from({ length: limits.r-limits.l+1 }, (v2, x) => 0) );    //y+','+x
    let startingPoint = {x: 500-limits.l, y: 0}

    for (let i = 0; i < input.length; i++) {
        for (let y = 0; y < input[i].length-1; y++) {
            grid = lines(input[i][y], input[i][y+1], grid, limits)
        }
    }
    //console.table(grid)
    
    let i = 0
    while(true){
        let sand = moveSand({x:startingPoint.x, y:startingPoint.y}, grid)
        
        if(sand.sandOverflow == true){
            let printStr = getPrintStr(grid, sand.path)
            return {grid, path:sand.path, restingCorns: i, printStr}
        } 

        grid[sand.pos.y][sand.pos.x] = 'o'

        if(sand.pos.y==startingPoint.y && sand.pos.x==startingPoint.x){
            let printStr = getPrintStr(grid, [])
            return {grid, path:sand.path, restingCorns: i+1, printStr}
        }

        i += 1
        //if(i > 100) break
    }
    
    return {grid, path:[], restingCorns: i, printStr:''}
}


function moveSand(pos, grid, path = [], sandOverflow = false){
    path.push(pos)
    let xMax = grid[0].length-1
    let yMax = grid.length-1

    let moves = [
        {y: pos.y+1, x:pos.x},
        {y: pos.y+1, x:pos.x-1},
        {y: pos.y+1, x:pos.x+1}
    ]
    for (let i = 0; i < moves.length; i++) {
        if(moves[i].y > yMax || moves[i].x > xMax || moves[i].x < 0)
            return {pos, path, sandOverflow:true}
        else if(grid[moves[i].y][moves[i].x] == 0)
            return moveSand({x:moves[i].x, y:moves[i].y}, grid, path)
    }
    return {pos, path, sandOverflow:false}
}

function lines(lineStart, lineEnd, grid, limits){
    let diff = {x: lineEnd.x-lineStart.x, y: lineEnd.y-lineStart.y}
    for (let i = 0; i < Math.abs(diff.y)+Math.abs(diff.x)+1; i++) {
        let yy = lineStart.y+Math.sign(diff.y)*i
        let xx = Math.abs(lineStart.x-limits.l+Math.sign(diff.x)*i)
        grid[yy][xx] = `\u2588`//'#'        
    }
    return grid
}


function getPrintStr(grid, failPath){
    let str = ''
    for (let y = 0; y < grid.length; y++) {
        for (let x = 0; x < grid[y].length; x++) {
            let cell = grid[y][x]
            if(cell==0 || cell==' ' || cell=='.') str +=  utils.rgb(0,0,0, 'fg')
            if(cell=='o' || cell=='s') str +=  utils.rgb(0,155,255)
            if(cell==`\u2588` || cell==`#`) str +=  utils.rgb(255,0,255)

            for (let i = 0; i < failPath.length; i++) {
                if(failPath[i].x == x && failPath[i].y == y)
                    str +=  utils.rgb(0,255,0, 'bg')
            }
            str += cell + utils.reset()
        }
        str += `\n`
    }
    return str
}

let p1 = init()
console.log(p1.printStr)
//utils.writeFile(`\``+p1.printStr+`\``, `day${day}.1_str`)

let p2 = init(true)
//console.log(p2.printStr)
//utils.writeFile(`\``+p2.printStr+`\``, `day${day}.2_str`)


const t1 = performance.now();console.log(utils.currentDateTime(), t1);
console.log(`${utils.rgb(0,255,0)}Day ${day}. \t Part1: ${p1.restingCorns} \t Part2: ${p2.restingCorns} \t\t ${Math.round((t1 - t0))} ms ${utils.reset()} `);
// Day 14.          Part1: 1072    Part2: 24659              456 ms 
