var ENV_PORT_PATH = process.env.SYABERUNO_PATH;
if(typeof ENV_PORT_PATH == "undefined"){
	console.log("ERROR : env SYABERUNO_PATH not exported!!");
	process.kill();
}


var SerialPort = require("serialport").SerialPort
var serialPort = new SerialPort(ENV_PORT_PATH, {
	baudrate: 9600,
	databits: 8,
	stopbits: 1,
	parity: 'none',
	flowcontrol: false,
	buffersize: 255,
	parser: require("serialport").parsers.readline("\n")
});

if(typeof process.argv[2] == "undefined"){
	message = "defo^rutome^sse-gi";
}else{
	message = process.argv[2];
}

serialPort.on("open", function () {
	var write = function(str){
		setTimeout(function(str){
			serialPort.write(str + "\r", function(err, results) {
				console.log('"' + str + '" ' + 'SENDED!! (sent.)');
				process.kill();
			}); 
		},500,str);
	}

	write(message);
});
