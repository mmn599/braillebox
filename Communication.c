#include "FIFO.h"
#include "UART.h"
#include "BrailleConverter.h"
#include "BrailleReaderMain.h"
#include "ZigBee.h"
#include "ST7735.h"

#define STRINGMAX 100

BrailleString waitForString(void) {
	char string[STRINGMAX];
	int size = UART_InString(string, STRINGMAX);
	BrailleString bstring = createBrailleString(string,size);
	return bstring;
}

BrailleString getUARTString(void) {
	BrailleString string;
	char toprint[STRINGMAX];
	char i = 0;
	char size = 0;
	char done = 0;
	while(!done) {
		XBeeFrame frame = XBee_ReceiveFrame();
		char char1 = frame.frame[8];
		char char2 = frame.frame[9];
		toprint[i++] = char1;
		toprint[i++] = char2;
		if(char1==0) {
			done=1;
			size=i-1;
		}
		if(char2==0) {
			done=1;
			size = i;
		}
	}
	i=0;
	while(toprint[i]!=0) {
		toprint[i] = toprint[i];
		i++;
	}
	size = i;
	string = createBrailleString(toprint,size);
	return string;
}


BrailleString printUARTString(void) {
	BrailleString string;
	char toprint[STRINGMAX];
	char i = 0;
	char size = 0;
	char done = 0;
	while(!done) {
		XBeeFrame frame = XBee_ReceiveFrame();
		char char1 = frame.frame[8];
		char char2 = frame.frame[9];
		toprint[i++] = char1;
		toprint[i++] = char2;
		if(char1==0) {
			done=1;
			size=i-1;
		}
		if(char2==0) {
			done=1;
			size = i;
		}
	}
	i=0;
	while(toprint[i]!=0) {
		toprint[i] = toprint[i];
		i++;
	}
	size = i;
	int y;
	for(y=0;y<size;y++) {
		ST7735_OutChar(toprint[y]);
	}
	string = createBrailleString(toprint,size);
	return string;
}

void communicationInit() {
	UART_Init();
	XBee_Init_Receive();
}



void OutCRLF(void){
  UART_OutChar(CR);
  UART_OutChar(LF);
}