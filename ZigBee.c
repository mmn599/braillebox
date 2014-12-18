/*
XBee ZigBee device driver.

Matthew Normyle and Kapil Gowru, 2014
*/

#include "UART.h"
#include "SysTick.h"
#include "Zigbee.h"

const char PACKETID = 1;

const char string0[10] = "+++";
//destination address
const char string1[10] = {'A','T','D','L','4','F',13,0};
const char string2[10] = {'A','T','D','H','0',13,0};
//home address!
const char string3[10] = {'A','T','M','Y','4','E',13,0};
const char string4[10] = {'A','T','A','P','1',13,0};
const char string5[10] = {'A','T','C','N',13,0};

//destination address
const char string1receive[10] = {'A','T','D','L','4','E',13,0};
//home address
const char string3receive[10] = {'A','T','M','Y','4','F',13,0};


void XBee_Init() {
	int i;
	UART_OutChar('X');
	SysTick_Wait10ms(110);
	UART_OutString((char*)string0);
	SysTick_Wait10ms(110);
	UART_OutString((char*)string1);
	SysTick_Wait10ms(2);
	UART_OutString((char*)string2);
	SysTick_Wait10ms(2);
	UART_OutString((char*)string3);
	SysTick_Wait10ms(2);
	UART_OutString((char*)string4);
	SysTick_Wait10ms(2);
	UART_OutString((char*)string5);
	SysTick_Wait10ms(2);
}

void XBee_Init_Receive(void) {
	int i;
	UART_OutChar('X');
	SysTick_Wait10ms(110);
	UART_OutString((char*)string0);
	SysTick_Wait10ms(110);
	UART_OutString((char*)string1receive);
	SysTick_Wait10ms(2);
	UART_OutString((char*)string2);
	SysTick_Wait10ms(2);
	UART_OutString((char*)string3receive);
	SysTick_Wait10ms(2);
	UART_OutString((char*)string4);
	SysTick_Wait10ms(2);
	UART_OutString((char*)string5);
	SysTick_Wait10ms(2);
}

//returns an 11 char array
XBeeFrame XBee_CreateFrame(char char1, char char2, char destination) {
	XBeeFrame frame;
	//START
	frame.frame[0] = 0x7E;
	//LENGTH 
	frame.frame[1] = 0x00;
	frame.frame[2] = 0x07;
	//API
	frame.frame[3] = 0x01;
	//ID
	frame.frame[4] = 0x01;
	//DESTINATION
	frame.frame[5] = 0x00;
	frame.frame[6] = destination;
	//OPT
	frame.frame[7] = 0x00;
	//MESSAGE
	frame.frame[8] = char1;
	frame.frame[9] = char2;
	//checksum
	frame.frame[10] = 0xFF-(frame.frame[3]+frame.frame[4]+frame.frame[5]+frame.frame[6]+frame.frame[7]+frame.frame[8]+frame.frame[9]);
	return frame;
}

const char SUCCESS[5] = {0x00,0x03,0x89,PACKETID,0x00};

int XBee_TxStatus(void) {
	char current = UART_InChar();
	while(current!=0x7E) {
		current = UART_InChar();
	}
	int i;
	for(i=0;i<5;i++) {
		current = UART_InChar();
		if(current!=SUCCESS[i]) {
			return 0;
		}
	}
	unsigned char checksum = 0xFF - (SUCCESS[2] + SUCCESS[3] + SUCCESS[4]);
	if(UART_InChar()!=checksum) {
		return 0;
	}
	return 1;
}

int XBee_SendFrame(XBeeFrame frame) {
	int i;
	for(i=0;i<11;i++) {
		UART_OutChar(frame.frame[i]);
	}
	for(i=0;i<1000;i++) {};	
	return XBee_TxStatus();
}

XBeeFrame XBee_ReceiveFrame() {
	XBeeFrame frame;
	int i;
	char current = UART_InChar();
	while(current!=0x7E) {
		current = UART_InChar();
	}
	frame.frame[0] = current;
	for(i=1;i<11;i++) {
		frame.frame[i] = UART_InChar();
	}
	return frame;
}