/**
 * HTTP通信部分を抜いた簡易版
 */
#include <SPI.h>
#include <AquesTalk.h>

AquesTalk atp;

void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(9600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}
}


void loop() {
	serialListen();
}

//Arduinoに渡す文字列のバッファ
char text[200];
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

