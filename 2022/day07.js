const fs = require('fs');

function readFileToArray(filename) {
	const contents = fs.readFileSync(filename, 'utf-8');
	const arr = contents.split(/\r?\n/);
	return arr;
}

let input_data = readFileToArray('./data/day07.txt');


function inputHandler(rowInput, currentDir, dir){
     if(rowInput[1] == 'cd'){     
        if(rowInput[2] == '/'){
            currentDir = '/root/'
        }         
        else if(rowInput[2] == '..'){
            let currentDirArray = currentDir.split('/')
            currentDirArray.splice(currentDirArray.length-2, 1)
            currentDir = currentDirArray.join('/')
            //currentDir = currentDir.replace(currentDir.split('/').at(-2)+'/', ''); 
        }
        else {
            currentDir = currentDir + rowInput[2] + '/'  
        }
                   
    } 
    else if(rowInput[1] == 'ls'){}
    return [dir, currentDir] 

}

function getCurrentdirObj(currentDirArr, dir, rowInput, i = -1, tt=''){
    i = i + 1
    if(!dir.hasOwnProperty(currentDirArr[i]))
        dir[currentDirArr[i]] = {}  

    if(i == currentDirArr.length-1 && rowInput[0] != 'dir')
        dir[currentDirArr[i]][rowInput[1]] = rowInput[0]
    else if(i < currentDirArr.length-1)
        dir[currentDirArr[i]] = getCurrentdirObj(currentDirArr, dir[currentDirArr[i]], rowInput, i)
    
    return dir
}

function getDirectoriesSize(dir, folderId = '/', summary = {}){ 
    if(!summary.hasOwnProperty(folderId))
        summary[folderId] = Number(0)

    for (const [key, value] of Object.entries(dir)) {
        if(typeof value == 'string')
            summary[folderId] = summary[folderId] + Number(value)
        else 
            summary[folderId] = summary[folderId] + getDirectoriesSize(value, folderId+key+'/', summary)[folderId+key+'/'] 
    }
    return summary
}

function getDirectoriesGreaterThan(sizeSummary, sizeLimit){
    let sum = 0
    for (const [key, value] of Object.entries(sizeSummary)) {
        if(Number(value) <= Number(sizeLimit)){
            sum += value
        }
    }
    return sum
}


class Part1 {
	constructor() {
		this.dirSum = 0;
        this.dirSummary = 0;
		this.iterate();
	}

	iterate() {
        let currentDir
        let dir = {}
        input_data.forEach(row => {
            let rowInput = row.split(' ')
            if(rowInput[0] == '$'){
                [dir, currentDir] = inputHandler(rowInput, currentDir, dir)
            }
            else if(rowInput[0] == 'dir'){}
            else{
                let currentDirArr = currentDir.split('/')
                currentDirArr.shift()
                currentDirArr.pop()
                dir = getCurrentdirObj(currentDirArr, dir, rowInput)
             }
        });
        this.dirSummary = getDirectoriesSize(dir) 
        this.dirSum = getDirectoriesGreaterThan(this.dirSummary, 100000)
        
	}
}

  








let p1 = new Part1(); // 1428881 
let p2 = Object.values(p1.dirSummary).filter((val) => val > (30000000-(70000000-p1.dirSummary['/']))).sort((a,b)=>a-b)[0]   // 10475598


console.log(`Part1: ${p1.dirSum} --- Part2: ${p2} `);
// Part1: 1428881 --- Part2: 10475598
