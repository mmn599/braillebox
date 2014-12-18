
#ifndef ZIGBEE_H
#define ZIGBEE_H


typedef struct XBeeFrame {
	
	char frame[11];
	
} XBeeFrame;


void XBee_Init(void);
void XBee_Init_Receive(void);
XBeeFrame XBee_CreateFrame(char char1, char char2, char destination);
int XBee_SendFrame(XBeeFrame frame);
int XBee_Status(void);
XBeeFrame XBee_ReceiveFrame();

#endif

