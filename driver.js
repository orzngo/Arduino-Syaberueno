
var SerialPort = require("serialport").SerialPort
var serialPort = new SerialPort("/dev/tty.usbserial-A6008hiA", {
	baudrate: 9600,
	databits: 8,
	stopbits: 1,
	parity: 'none',
	flowcontrol: false,
	buffersize: 255,
	parser: require("serialport").parsers.readline("\n")
});

if(typeof process.argv[2] == "undefined"){
	message = "hikisuuganaiyo";
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
