

let utils = require('./utils.js');
console.log = (str => process.stdout.write(`🍕 ${str}\n`))
const t = {start: performance.now()}


let day = 15
let example = false
let param = {
    dataset: example == true ? 1 : 2,
    y: example == true ? 10 : 2000000,
    zone: example == true ? 20 : 4000000
}
let input_data = utils.readFileToArray(`./data/day${day}.txt`, param.dataset, /\r\n/); 
let input = parse(input_data)

t['part1Start'] = performance.now()
let p1 = part1(input, param.y)
console.log(`Part1: ${utils.normalizeLength(p1, 15, ' ')} \t ${Math.round(performance.now()-t.part1Start)} ms`)    
// 🍕 Part1: 4725496                6600 ms

t['part2Start'] = performance.now()
let p2 = part2(input, param.zone)
console.log(`Part2: ${utils.normalizeLength(p2, 15, ' ')} \t ${Math.round(performance.now()-t.part2Start)} ms`)    
// 🍕 Part2: 12051287042458         1161 ms

console.log(`${utils.rgb(0,255,0)}Day ${day}. \t Part1: ${p1} \t Part2: ${p2} \t\t ${Math.round((performance.now() - t.start))} ms ${utils.reset()} `);
// 🍕 Day 15.       Part1: 4725496          Part2: 12051287042458           7764 ms  



function parse(data){
    return data.map(line => {
        let part = line.split(' ')
        let obj = {
            sensor: {
                x: Number(part[2].split('=').at(-1).replace(',','')),
                y: Number(part[3].split('=').at(-1).replace(':',''))
            },
            beacon: {
                x: Number(part[8].split('=').at(-1).replace(',','')),
                y: Number(part[9].split('=').at(-1))
            }
        }
        let diff = {
            x: Math.abs(obj.sensor.x - obj.beacon.x), 
            y: Math.abs(obj.sensor.y - obj.beacon.y)
        }
        obj.sensor['radius'] = Number(diff.x+diff.y)
        //console.log(obj)
        return obj
    })
}


function part1(input, y){
    let coordsAtY = new Set()
    for (let i = 0; i < input.length; i++) {
        coordsAtY = checkRange(input[i].sensor, input[i].beacon, y, coordsAtY)
    }
    let bacons = new Set() // ≈
    for (let i = 0; i < input.length; i++) {
        if(input[i].beacon.y == y)
            bacons.add(JSON.stringify(input[i].beacon))
    }
    return coordsAtY.size-bacons.size
}
function checkRange(s, b, y, coords){
    let radius = s.radius 
    let distanceToY = Number(Math.abs(s.y - y))
    if (distanceToY <= radius) {
        for (let x = Number(s.x)-(radius-distanceToY); x <=  Number(s.x)+(radius-distanceToY); x++) {
            coords.add(JSON.stringify({x, y}))
        }
    }
    return coords
}
function part2(input, zone){
    for (let y = 0; y <= zone; y++) {                                           // FOR y
        let parts = []
        for (let i = 0; i < input.length; i++) {                                // FOR sensors
            let s = input[i].sensor
            //let b = input[i].beacon
            let distanceToY = s.radius - Math.abs(s.y -y)
            if (distanceToY >= 0) {
                let xLim = {
                    min: s.x-distanceToY < 0 ? 0 : s.x-distanceToY,
                    max: s.x+distanceToY > zone ? zone : s.x+distanceToY
                }
                let used = false
                for (let p = 0; p < parts.length; p++) {                        // FOR parts
                    if( (xLim.min >= parts[p].f  && xLim.min <= parts[p].t) ||
                        (xLim.max >= parts[p].f  && xLim.max <= parts[p].t) ||
                        (xLim.min <= parts[p].f  && xLim.max >= parts[p].t)){
                        if(xLim.min < parts[p].f)
                            parts[p].f = xLim.min
                        if(xLim.max > parts[p].t)
                            parts[p].t = xLim.max

                        if(parts[p-1] && parts[p].f <= parts[p-1].t){
                            parts[p].f = parts[p-1].f
                            parts.splice(p-1, 2, parts[p]);
                        } 
                        else if(parts[p+1] && parts[p].t >= parts[p+1].f){
                            parts[p].t = parts[p+1].t
                            parts.splice(p, 2, parts[p]);
                        } 
                        used = true
                    } 
                }

                if(parts.length == 0)
                    parts.push({f:xLim.min,t:xLim.max})
                else if(used == false && xLim.min < zone && xLim.max > 0){
                    for (let p = 0; p < parts.length; p++) {
                        if(xLim.min < parts[p].f){
                            parts.splice(p, 0, {f:xLim.min,t:xLim.max});
                            break
                        }
                        else if(p == parts.length-1){
                            parts.push({f:xLim.min,t:xLim.max})
                            break
                        }
                    }
                }
            }
        }
        if(parts.length > 1)
            return Number(parts[0].t+1) * 4000000 + y
    }
}