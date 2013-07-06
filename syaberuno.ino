#include <SPI.h>
#include <Ethernet.h>
#include <AquesTalk.h>
#include <Wire.h>

byte mac[] = {
	0x00, 0x50, 0xC2, 0x97, 0x21, 0x83 };
IPAddress ip(192,168,1,252);
EthernetServer server(80);

AquesTalk atp;

void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(9600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}


	// start the Ethernet connection and the server:
	Ethernet.begin(mac, ip);
	server.begin();
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());
}


void loop() {
	ethernetListen();
	serialListen();
}

//Arduinoに渡す文字列のバッファ
char text[200];
/*------------------------------------
イーサネット部
--------------------------------------*/
void ethernetListen()
{
	// listen for incoming clients
	EthernetClient client = server.available();
	if (client) {
		Serial.println("new client");
		// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		String line = String("");
		int lineCount = 0;
		boolean isParamValied = false;
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				line.concat(String(c));
				Serial.write(c);

				// if you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended,
				// so you can send a reply
				if (c == '\n' && currentLineIsBlank) {
					// send a standard http response header
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println("Connection: close");  // the connection will be closed after completion of the response
					client.println();
					client.println("<!DOCTYPE HTML>");
					client.println("<html>");
					if(isParamValied){
						client.println(text);
						client.println(" SENDED!!(sent.)");
					}else{
						client.println("Unknown Syaberuno Request.");
					}
					client.println("</html>");
					break;
				}
				if (c == '\n') {
					// you're starting a new line
					currentLineIsBlank = true;
					lineCount++;
					if(lineCount == 1){
						//最初の１行が読み終わった
						if(line.indexOf("GET") != -1){
							getParam(line,text);
							isParamValied = true;
							atp.Synthe(text);  //引数に指定のテキストを発声
						}
					}
				}
				else if (c != '\r') {
					// you've gotten a character on the current line
					currentLineIsBlank = false;
				}
			}
		}
		// give the web browser time to receive the data
		delay(1);
		// close the connection:
		client.stop();
		Serial.println("client disonnected");
	}
}

//GETパラメータ取得します
void getParam(String line, char* textBuf)
{
	String param = line.substring(5,line.indexOf("HTTP")-1);
	param.toCharArray(textBuf, 200);
}

/*--------------------------------------
シリアル通信部
---------------------------------------*/
void serialListen(){
	int serialAvailable = Serial.available();
	if (serialAvailable > 0) { // シリアル通信でデータが送られてくるまで待つ。
		getsSerial(text);
		atp.Synthe(text);  //引数に指定のテキストを発声
		Serial.print("done.\n");
	}
}

void getsSerial(char* textBuf)
{
	int i = 0;
	char ch;
	while(1){
		if(Serial.available() > 0)
		{
			ch = Serial.read();
			if(ch == '\0' || ch == '\r') break;
			textBuf[i] = ch;
			Serial.println(ch);
			i++;
		}
	}
	textBuf[i] = '\0';
}

