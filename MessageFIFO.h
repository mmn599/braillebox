#define MESSAGEFIFOSIZE 8
#define SUCCESS 1
#define FAIL 0

void messageFifoInit(void);

int messageFifoPut(BrailleString data);

int messageFifoGet(BrailleString* string);

unsigned short messageFifoSize(void);
