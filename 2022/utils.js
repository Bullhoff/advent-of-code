const fs = require('fs');
const { Console } = require('node:console');


const folder = './output/'
if (!fs.existsSync(folder)) fs.mkdirSync(folder);
const logFolder = './output/logs/'
if (!fs.existsSync(logFolder)) fs.mkdirSync(logFolder);

const output = fs.createWriteStream(`${logFolder}stdout.log`);
const errorOutput = fs.createWriteStream(`${logFolder}stderr.log`);
const logger = new Console({ stdout: output, stderr: errorOutput });
 
module.exports.logger = function(text, datetime = this.currentDateTime()+'\t') {
	//console.log(typeof text)
	if(typeof text == 'object')
		JSON.stringify(text, null, 2)
	logger.log(`${datetime}${text}`);
}

module.exports.readFileToArray = function(filename, part = 'all', splitter = /\r?\n/) {
	const contents = fs.readFileSync(filename, 'utf-8');		
	if(part == 'all')
		return contents.split(splitter);
	else 
		return contents.split('\r\n-----SEPARATOR-----\r\n')[part].split(splitter);
}

module.exports.writeFile = function(content,filename = 'test', ext = '.txt') {
	//console.log(typeof content)
	if(typeof content == 'number') content = content.toString()
	else if(typeof content == 'object') content = JSON.stringify(content, null, 2)

	fs.writeFile(`${folder}${filename}${ext}`, content, (err) => {
		if (err) {console.error(err);}
	});
}

module.exports.sortArray = function(arr, sortBy = null) {
	if((sortBy == null && typeof arr[0] == 'string') || sortBy == 'string')
		return arr.sort(function (a, b) {return b.length - a.length;});
	else if((sortBy == null && typeof arr[0] == 'number') || sortBy == 'number')
		return arr.sort((a,b)=>a-b);
}
module.exports.sortArrayObject = function(arr, key, sortBy = null) {
	if((sortBy == null && typeof arr[0][key] == 'string') || sortBy == 'string')
		return Object.values(arr).sort((a,b)=>b[key].length-a[key].length)
	else if((sortBy == null && typeof arr[0][key] == 'number') || sortBy == 'number')
		return Object.values(arr).sort((a,b)=>a[key]-b[key])
	
}

module.exports.rgb = function(r = 0, g = 0, b = 0, where = 'fg'){
	let code
	if(where == 'bg') code = 48
	else if(where == 'fg') code = 38
    return `\x1B[${code};2;${r};${g};${b}m`
}
module.exports.reset = function(){
	return `\x1b[0m`
}

module.exports.onlyUnique = function(value, index, self) {
	return self.indexOf(value) === index;
}

module.exports.hasDuplicates = (arr) => arr.length !== new Set(arr).size;

module.exports.operators = {
    '+': function(a, b) { return a + b },
    '-': function(a, b) { return a - b },
    '*': function(a, b) { return a * b },
    '/': function(a, b) { return a / b }
}


module.exports.normalizeLength = ((text, paddingLength = 2, padding = '.')=> {
	if(text.toString().length == Math.abs(paddingLength)) return text
	else if(paddingLength < 0) return this.normalizeLength((padding+text).substr(paddingLength), paddingLength, padding)
	else if (paddingLength > 0) return this.normalizeLength((text+padding).substr(0,paddingLength), paddingLength, padding)
});

let twoNumbers = ((number)=> ('0'+number).substr(-2));
module.exports.currentDateTime = function (date_time_datetime = 'datetime', separators = ['-', ' ', ':']) {
	let dateTime = new Date(Date.now());
	//if (!dateTime) return;
	let [year, month, day] = [dateTime.getFullYear(), twoNumbers(dateTime.getMonth()+1), twoNumbers(dateTime.getDate())];
	let [hours, minutes, seconds, ms] = [twoNumbers(dateTime.getHours()), twoNumbers(dateTime.getMinutes()), twoNumbers(dateTime.getSeconds()), dateTime.getMilliseconds()];

	let date_str = year + separators[0] + month + separators[0] + day;
	let time_str = hours + separators[2] + minutes + separators[2] + seconds;
	let dateTime_str = date_str + separators[1] + time_str;

	if (date_time_datetime == 'date') return date_str;
	else if (date_time_datetime == 'time') return time_str;
	else if (date_time_datetime == 'yyyyMM') return year + separators[0] + month;
	else return dateTime_str;
};


module.exports.replaceAll = function (str, replace = '\"', replaceWith = '') {
	return str.replace(new RegExp(replace, 'g'), replaceWith)
}


module.exports.html = function({body='',bodyTag='', style='', styleExternal='<link rel="stylesheet" href="styles.css">', scriptExternal='', script=''}) {
    return `
	<!DOCTYPE html>
	<html>
	<head>
		<script>${script}</script>
		<style>${style}</style>
		${styleExternal}
	</head>
	<body ${bodyTag}>
		${body} 
	</body>
	</html>`
}


/* 

 */