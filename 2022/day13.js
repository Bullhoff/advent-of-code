

let utils = require('./utils.js');
const t0 = performance.now();
console.log(utils.currentDateTime(), t0)
let day = 13
let input_data = utils.readFileToArray(`./data/day${day}.txt`, 1, /\r\n\s*\r\n/);   /// \n\s*\n/   // /\r\n\s*\r\n/
console.table(input_data)


let startPos,goalPos
//let pairs = input_data.map((row, y) => row.split('\r\n'));
let pairs = input_data.map((i) => i.split('\r\n').map((y) => JSON.parse(y)));
let orgPairs = JSON.parse(JSON.stringify(pairs))
//console.table(pairs)
//console.table(pairs[1])
//console.table(pairs[1][0])
//console.log(JSON.parse(pairs[0][0]))
//console.log(JSON.parse(1))
console.log(Array.isArray(pairs[0]), Array.isArray(1), typeof pairs[0], typeof pairs[0][0])
let toPrint = []
//let printable = grid.map(x => x.join(''))
//console.log(printable)


/*
Left side smaller
Left side ran out of items, so inputs are in the right order
Right side ran out of items, so inputs are not in the right order

*/
let correctPairs = []
let allRes = []
aa()
function aa(){
    
    //let orgPairs = [...pairs]
    
    for (let pair_i = 0; pair_i < pairs.length; pair_i++) {
        //console.log(pair_i,pairs[pair_i][0], '\n', pair_i,pairs[pair_i][1])
        let res = []
        let rightOrder = compare(pairs[pair_i][0], pairs[pair_i][1], 'status',res)
        allRes.push(res)
        //if(!rightOrder[2].includes('wrong')) correctPairs.push(pair_i+1)
        console.log(pair_i,'**--**',res, rightOrder, correctPairs.length, JSON.stringify(rightOrder), '\t', 
        '\nPair1:'+JSON.stringify(pairs[pair_i][0]), '\nPair2:'+JSON.stringify(pairs[pair_i][1]),
        '\norgPair1:'+JSON.stringify(orgPairs[pair_i][0]), '\norgPair2:'+JSON.stringify(orgPairs[pair_i][1]), 
        '\nres:'+JSON.stringify(res))
        if(res[0].includes('wrong')) {
            correctPairs.push(pair_i+1)
            console.log('***',pair_i, correctPairs.length, JSON.stringify(rightOrder), '\t', '\nPair1:'+JSON.stringify(pairs[pair_i][0]), '\nPair2:'+JSON.stringify(pairs[pair_i][1]))
            console.log(JSON.stringify(res))
        }
        if(!rightOrder[2].includes('wrong')){
            
        }
        if(!rightOrder[2].includes('wrong')) {
            //utils.logger(`${pair_i} ${JSON.stringify(pairs[pair_i][0])} \t ${JSON.stringify(pairs[pair_i][1])}`)
            utils.logger(`${pair_i}__ ${JSON.stringify(rightOrder[2])} \t ${JSON.stringify(rightOrder[0])} \t ${JSON.stringify(rightOrder[1])}`)
            utils.logger(`${pair_i}_0 ${JSON.stringify(orgPairs[pair_i][0])}`)
            utils.logger(`${pair_i}_1 ${JSON.stringify(orgPairs[pair_i][1])}`)
            }

        //if(pair_i==2)
        //    break
    }
    //const sum = [1, 2, 3].reduce((partialSum, a) => partialSum + a, 0);
    const sum = correctPairs.reduce((partialSum, a) => partialSum + a, 0);
    console.log('correctPairs:',correctPairs, sum)
}
function compareNr(){}
function compare(left, right, status = 'okk',res = []){
    console.log('compare: ',typeof left, left, typeof right,right, status,res, left == undefined, right == undefined/* left.length, right.length */)
    while(true /* left.length != 0 && right.length != 0 */) {//true /* !status.includes('wrong') */ /* && left.length != 0 */{
        let [leftType, rightType] = [typeof left, typeof right]
        if(leftType == 'number' && left != undefined)
            left = [left]        
        if(rightType == 'number' && right != undefined)
            right = [right]
        
        //let before = JSON.stringify(res)
        //if(right == undefined || left == undefined)
        //    console.log('!!!!',right,left)
        if(right == undefined || left == undefined){
            if(right == undefined && left != undefined)
                res.push('wrong')
            else if (right != undefined && left == undefined)
                res.push('right')
            else 
                res.push('--undefined-')
            break
        }

        else if(right.at(0) == undefined || left.at(0) == undefined){
            if(right.at(0) == undefined && left.at(0) != undefined)
                res.push('wrong')
            else if (right.at(0) != undefined && left.at(0) == undefined)
                res.push('right')
            else 
                res.push('--undefined.at(0)-')
            break
            left.shift()
            right.shift()
        }
        //else if ((right.at(0) == undefined && left.at(0) == undefined) || right.at(0) == left.at(0) || (right.at(0) == null && left.at(0) == null))
        //    res.push('--')//return [left, right, 'right']
        else{
            if(typeof left.at(0) == 'object' || typeof right.at(0) == 'object'){
                //console.log('0obj: ', left, right, status, res)
                //return compare(left.at(0), right.at(0), status, res)
                [left, right, status, res] =  compare(left.at(0), right.at(0), status, res)
                //console.log('1obj: ', left, right, status, res)
            }
            else if (right.at(0) < left.at(0)) 
                res.push('wrong')
            else if (right.at(0) > left.at(0)) 
                res.push('right')
            left.shift()
            right.shift()
        }
        
    }
    return [left, right, status, res]
}



// 4304 --> too low
// 6545 --> too high
// 6398 --> too high
// 5009 --> 


let utilshtml = require('./utilshtml.js');
let table2 = `<div class="day13">` + pairs.map((i, ii) => {
    let result = `<div id="c0"><div id="c1">[${ii}] RES</div><div id="c2">${JSON.stringify(allRes[ii], null, 2)}</div></div>`
    return i.map((y, yy) => {
        let org = `<div id="c0"><div id="c1">[${ii}][${yy}] Org</div><div id="c2">${JSON.stringify(orgPairs[ii][yy],null,1)}</div></div>`
        let notOrg = `<div id="c0"><div id="c1">[${ii}][${yy}] notOrg</div><div id="c2">${JSON.stringify(y,null,1)}</div></div>`
        return ``+notOrg +org+``
    }).join('')+result
}).join('') +`</div>`;
let fullhtml = utilshtml.html(table2)

utils.writeFile(fullhtml, 'testhtml', '.html')


let _table2 = `<div class="day13">` + pairs.map((i) => {
    return i.map((y) => {
        return `<div>${JSON.stringify(y,null,1)}</div>`
    }).join('')
}).join('') +`</div>`;

function getCells(data, type) {
  return data.map(cell => `
    <${type}>
        ${JSON.stringify(cell)}
    </${type}>`).join('');
  //return data.map(cell => `<${type}>${cell}</${type}>`).join('');
}

function createBody(data) {
  return data.map(row => `
        ${getCells(row, 'div')} 
    `);
}

function createTable(data) {
  const [headings, ...rows] = [['p1', 'p2'],data];  //${getCells(headings, 'th')} 
  return `
    <div class="grid2">
                 ${createBody(rows)}
      
             
      
    </div>
  `;
}


/* let data = [
  ['Title', 'Artist', 'Duration', 'Created'],
  ['hello', 'me', '2', '2019'],
  ['ola', 'me', '3', '2018'],
  ['Bob', 'them', '4.3', '2006']
]; */
/* 
function getCells(data, type) {
  return data.map(cell => `
    <${type}>
        ${JSON.stringify(cell)}
    </${type}>`).join('');
  //return data.map(cell => `<${type}>${cell}</${type}>`).join('');
}

function createBody(data) {
  return data.map(row => `
    <tr>
        ${getCells(row, 'td')}
    </tr>`).join('');
}

function createTable(data) {
  const [headings, ...rows] = [['p1', 'p2'],data];
  return `
    <table>
      <thead>        ${getCells(headings, 'th')}
      </thead>
      <tbody>        ${createBody(rows)}
      </tbody>
    </table>
  `;
} */

// Bang it altogether
//document.body.insertAdjacentHTML('beforeend', createTable(data));
//document.getElementById("demo").innerHTML = "I have changed!";