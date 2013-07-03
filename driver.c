#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <sys/wait.h>

#define BAUDRATE B9600

/* 適切なシリアルポートを指すように，この定義を変更
 * 我が家の環境ではArduinoは/dev/ttyACM0になってました*/
#define MODEMDEVICE "/dev/ttyACM0"

#define BUFFSIZE 256
#define COULDNOTFORK -1

#define FALSE 0
#define TRUE 1

volatile int STOP = FALSE;
static int fd = -1;

/* Functions */
void serial_init(int fd);
void child_process();
void parent_process(pid_t result_pid);

/* Main */
int main(void);

// シリアルポートの初期化
void serial_init(int fd) {
	struct termios tio;
	memset(&tio, 0, sizeof(tio));
	tio.c_cflag = CS8 | CLOCAL | CREAD;
	/*
BAUDRATE: ボーレートの設定．cfsetispeed と cfsetospeed も使用できる．
CS8     : 8n1 (8 ビット，ノンパリティ，ストップビット 1)
CLOCAL  : ローカル接続，モデム制御なし
CREAD   : 受信文字(receiving characters)を有効にする．
*/

	tio.c_cc[VTIME] = 0; /* キャラクタ間タイマは未使用 */

	/*
ICANON  : カノニカル入力(行単位入力）を有効にする
*/
	tio.c_lflag = ICANON;

	/*
IGNPAR  : パリティエラーのデータは無視する
ICRNL   : CR を NL に対応させる(これを行わないと，他のコンピュータで
CR を入力しても，入力が終りにならない)
それ以外の設定では，デバイスは raw モードである(他の入力処理は行わない)
*/
	tio.c_iflag = IGNPAR | ICRNL;

	// ボーレートの設定
	cfsetispeed(&tio, BAUDRATE);
	cfsetospeed(&tio, BAUDRATE);
	// デバイスに設定を行う
	tcsetattr(fd, TCSANOW, &tio);
}

void child_process() {

	char buf[BUFFSIZE];
	char input[BUFFSIZE];
	int count;
	int i;
	int writecount = 0;

	fprintf(stdout, "CHILD:RCV Start\n");

	//STOPになるまで無限ループ
	while (FALSE == STOP) {
		memset(&buf, 0, sizeof(buf));
		count = read(fd, &buf, BUFFSIZE);
		if (count < 0) {
			fprintf(stdout, "CHILD:Could not read from serial port\n");
			STOP = TRUE;
		} else {
			fprintf(stdout, "CHILD:RCVD CHAR %s %d", buf, count);

		}
	}

	fprintf(stdout, "CHILD:BYE!\n");
}

void parent_process(pid_t result_pid) {

	char input[BUFFSIZE];
	int writecount = 0;
	fprintf(stdout, "Parent:Waiting for Input\n");
	int i = 0;
	int inputcount = 0;
	while (1) {
		memset(&input, 13, sizeof(input));

		fgets(input, sizeof(input), stdin);
		fflush(stdin);

		//改行コード埋め込み
		for (i = 0; i < BUFFSIZE; i++) {
			if (input[i] == 0) {
				inputcount = i;
				input[i] = 13;
				input[i + 1] = 10;
				break;
			}
		}

		writecount = write(fd, &input, inputcount);
		if (writecount < 0) {
			fprintf(stdout, "Could not write to serial port %d\n", writecount);
			break;
		} else {
			fprintf(stdout, "Send %d bytes\n", writecount);

		}

	}
}

int main(void) {
	char *devicePath = getenv("SYABERUNO_PATH");
	if(devicePath == NULL){
		perror("env SYABERUNO_PATH not exported!!");
		return 1;
	}
	pid_t result_pid;

	struct termios oldtio, newtio;
	char buf[255];
	/*
	   読み書きのためにモデムデバイスをオープンする．ノイズによって CTRL-C
	   がたまたま発生しても接続が切れないように，tty 制御はしない．
	   */

	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		perror(MODEMDEVICE);
		return (-1);
	}

	tcgetattr(fd, &oldtio); /* 現在のシリアルポートの設定を待避させる*/
	memset(&newtio, 0, sizeof(newtio));/* 新しいポートの設定の構造体をクリアする */

	//シリアルポート準備
	serial_init(fd);

	//forkして受信用と送信用に分ける
	result_pid = fork();

	if (result_pid == -1) {
		fprintf(stderr, "fork failed.\n");
		return COULDNOTFORK;
	}

	if (result_pid == 0) {
		child_process();
	} else {
		fprintf(stderr, "fork completed");

		parent_process(result_pid);
	}
	STOP = TRUE;

	return 0;
}

