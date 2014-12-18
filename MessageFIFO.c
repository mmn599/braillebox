#include "BrailleConverter.h"
#include "MessageFIFO.h"

unsigned long volatile PutI;
unsigned long volatile GetI;
BrailleString static MessageFifo[MESSAGEFIFOSIZE];

void messageFifoInit(void) {
	PutI = GetI = 0;
}

int messageFifoPut(BrailleString data) {
	if((PutI-GetI) & ~(MESSAGEFIFOSIZE-1)) {
		return FAIL;
	}
	MessageFifo[PutI&(MESSAGEFIFOSIZE-1)] = data;
	PutI++;
	return SUCCESS;
}

int messageFifoGet(BrailleString* string) {
	if(PutI==GetI) {
		return FAIL;
	}
	*string = MessageFifo[GetI&(MESSAGEFIFOSIZE-1)];
	GetI++;
	return SUCCESS;
}

unsigned short messageFifoSize(void) {
	return ((unsigned short)(PutI-GetI));
}
