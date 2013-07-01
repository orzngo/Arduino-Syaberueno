#include <AquesTalk.h>
#include <Wire.h>  // I2CライブラリAquesTalkライブラリ内部で使用するので定義必要


// Hello Talk - AquesTalk pico LSIで「こんにちは」を繰り返し発声する

AquesTalk atp;  //インスタンス定義 変数名は任意
int loopCount;

void setup()
{
	Serial.begin(9600);
	loopCount = 0;
}

void loop()
{
	loopCount++;
	int serialAvailable = Serial.available();
	if (serialAvailable > 0) { // シリアル通信でデータが送られてくるまで待つ。
		char text[200];
		getsSerial(text);
		atp.Synthe(text);  //引数に指定のテキストを発声
		Serial.print("done.\n");
	}
	delay(10);
	if(loopCount == 100){
		Serial.print("waiting\n");
		loopCount = 0;
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

