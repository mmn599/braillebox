/*
	A header for the char to braille converter file
*/

#ifndef BRAILLECONVERTER_H
#define BRAILLECONVERTER_H

typedef struct Braille {
	int output[6];
} Braille;

typedef struct BrailleString {
	Braille brailleString[100];
	char string[100];
	unsigned int size;
} BrailleString;


BrailleString createBrailleString(char* characters, int size);
void printBraille(BrailleString input);
void setSpeed(int);

#endif