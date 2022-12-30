// node day4.1.js


const { Console } = require('console');
const fs = require('fs');

let point_sums = []

let obj = {
    order: [],
    boards: [   // Board
        [       // Row
            //{
            //    number:[], // Each number in the row
            //    points:0   // Current points of the row
            //}
        ]
    ]    // [Board[ row[ NR[] ]]]
    //boards: [[]]    // [Board[ row[ NR[] ]]]
    //boards: [[1,2], [], []]
}
console.log(obj);

function syncReadFile(filename) {
    const contents = fs.readFileSync(filename, 'utf-8');
  
    const arr = contents.split(/\r?\n/);
    //const arr = contents.split(/\n\s*\n/);
    //console.log(arr); // 👉️ ['One', 'Two', 'Three', 'Four']
  
    return arr;
}
//let input_data = []
let input_data = syncReadFile('./data/day4.txt');
//console.log(input_data);


let board_count = 0
//let row_count = 0
console.log("LOOP");
for (let i = 0; i < input_data.length; i++) {
    
    //console.log(input_data[i])
    if (i == 0)
        obj["order"] = input_data[0].split(',');
    else if (input_data[i] == '' && i>1){
        //point_sums.push(0)
        //row_count = 0;
        board_count++;
        obj["boards"].push([]);
    }
    else if (input_data[i] != ''){
        //obj["boards"][board_count][row_count] = input_data[i].split(' ');
        //obj["boards"][board_count][row_count] = obj["boards"][board_count][row_count].filter((a) => a);

        //obj["boards"][board_count][row_count] = input_data[i].split(' ').filter((a) => a)
        //obj["boards"][board_count].push(input_data[i].split(' ').filter((a) => a))
        let temp = {
            number: input_data[i]
                        .split(' ')
                        .filter((a) => a),
            points: 0
        }
        obj["boards"][board_count].push(
            {
                row_numbers: input_data[i]
                    .split(' ')
                    .filter((a) => a),
                row_points: 0,
                column_points: 0,
                row_match_found:[], 
                col_match_found:[]
            }
        )
        
        console.log(obj["boards"][board_count][0])
        console.log(obj["boards"][0][0].row_numbers[0])
        /*
        obj["boards"][board_count].push(
            input_data[i]
                .split(' ')
                .filter((a) => a)
        )
        */
        //row_count++
    }
    
}

console.log(obj["boards"])
console.log("order: " + obj["order"])

let kebab = 0
let found_winner = false
let winner = ''
for (let i = 0; i < obj["order"].length; i++) {     // Loop drawn numbers   x
    //console.log("Checking: " + obj["order"][i]);
    
    
    for (let y = 0; y < obj["boards"].length; y++) { // Loop boards y
        //let found_number = false
        //point_sums[y] += parseInt(obj["order"][i])
        for (let z = 0; z < obj["boards"][y].length; z++) {    // Loop Rows z
            //console.log("MHMMMM " + obj["boards"][0][0].row_numbers[0])
            //console.log(obj["boards"][0][0].row_numbers[0])
            //console.log(obj["boards"][y].row_numbers[z])
            //console.log(obj["boards"][y][z])
            //console.log(obj["boards"][y][z]["row_points"])
            //console.log(obj["boards"][y][z]["row_numbers"][z])
            //obj["boards"][y].row_points_sum += parseInt(obj["order"][i])
            //console.log(obj["boards"][y][z].row_points_sum + "(" + parseInt(obj["order"][i]) + ") " + y)

            for (let n = 0; n < obj["boards"][y][z]["row_numbers"].length; n++) {    // Loop Numbers n
                //obj["boards"][y][z].match_found.push(false);
                obj["boards"][y][z].row_match_found[n] = false;
                if(obj["order"][i] == obj["boards"][y][z].row_numbers[n]){
                    obj["boards"][y][z].row_match_found[n] = true;
                    //obj["boards"][y].row_points_sum -= parseInt(obj["order"][i])
                    //obj["boards"][y][z].row_points_sum -= parseInt(obj["order"][i])
                    //console.log(obj["boards"][y][z].row_points_sum + "(" + parseInt(obj["order"][i]) + ") " + y)

                    obj["boards"][y][z].row_points ++;
                    obj["boards"][y][n].column_points ++; // here
                    obj["boards"][y][z]["row_numbers"][n] = "00"

                    console.log(obj["boards"][y][z].row_points + " - " + obj["boards"][y][n].column_points + " --- Row:" + z + " Col:"+n + "  *" + y + "  *"+obj["order"][i]) //46
                    //console.log(i +"("+obj["order"][i]+") Board:"+ y +" Row:"+ z +" n:"+n + " obj:"+ obj["boards"][y][z].row_numbers[n] + " **POINTS:"+obj["boards"][y][z].row_points)
                    //found_number == true;
                    if (
                        (obj["boards"][y][z].row_points == obj["boards"][y][z]["row_numbers"].length) ||
                        (obj["boards"][y][n].column_points == obj["boards"][y][z]["row_numbers"].length)
                        ){
                        
                        console.log("WINNER!!" + y + " - " + z + " Winning nr: " + obj["order"][i] + " row_points:"+obj["boards"][y][z].row_points + " column_points:" + obj["boards"][y][n].column_points)
                        //console.log(i +"("+obj["order"][i]+") Board:"+ y +" Row:"+ z +" n:"+n + " obj:"+ obj["boards"][y][z].row_numbers[n] + " **POINTS:"+obj["boards"][y][z].row_points)
                        found_winner = true;
                        winner = y
                        //console.log(obj["boards"][y])
                        calculate_points(obj["boards"][y], z, obj["order"][i], y)
                    } 
                    break;
                } else {
                    //point_sums[y] += parseInt(obj["order"][i]);
                    //console.log(point_sums[y] + "(" + parseInt(obj["order"][i]) + ") " + y)
                    //obj["boards"][y][z].match_found = false;
                }
            }
            if(found_winner == true)
                break;
        } 
        if(found_winner == true)
            break;
        
    }
    console.log("..."+winner + "-----"+obj["boards"][0][0]["row_numbers"].length +"-----"+found_winner)
    if(found_winner == true){
        //console.log("Winner:"+winner)
        break;
    }
            
}


function calculate_points(winner, winning_row, winning_number, tempwinner) {
    console.log("tempwinner"+tempwinner)
    kebab += 1
    let sum = 0;
    console.log("calculate_points" + " - " + winner.length+ " - " + winning_row+ " - " + winner.length)
    console.log(winner)
    //obj["boards"][y][z].row_numbers[n]
    //obj["boards"][y][z].match_found[n]
    //var s = `hello ${my_name}, how are you doing`;
    for (let row = 0; row < winner.length; row++) {
        //console.log(winner[row].row_points_sum)
        //sum += parseInt(winner[row].row_points_sum);

        for (let n = 0; n < winner[row]["row_numbers"].length; n++) {
            //sum += winner[row].row_numbers[n];
            //sum =  sum+ winner[row].row_numbers[n];
            //if (winner[row]["match_found"][n] != true){
                sum += parseInt(winner[row]["row_numbers"][n])
                var s = `*sum:${sum} *row:${row} *n:${n} *row_numbers:${winner[row]["row_numbers"][n]} *match_found:${winner[row]["row_match_found"][n]} *winning_number:${winning_number}`;
                //console.log(s)
            //}
            //sum += parseInt(winner[row].row_numbers[n]);
            //console.log(winner[row].row_numbers[n])
        }
        /*
        if(row != winning_row){
            console.log("calculate_points" + " - " + winner.length+ " - " + winning_row+ " - " + winner[row]["row_numbers"].length)
            for (let n = 0; n < winner[row]["row_numbers"].length; n++) {
                //sum += winner[row].row_numbers[n];
                //sum =  sum+ winner[row].row_numbers[n];
                sum += parseInt(winner[row].row_numbers[n]);
                console.log(winner[row].row_numbers[n])
            }
            
        }
        */
    }
    //console.log(winning_number)
    //console.log(winner)
    console.log(`*sum:${sum} *winning_number:${winning_number} *Result:${sum*winning_number} *winning_row:${winning_row} *kebab:${kebab}`)
    
}

/*

Answer day4.1 --> 52965 --> That's not the right answer; your answer is too low.

119394 --> That's not the right answer; your answer is too high.

69579 --> That's the right answer!
*/