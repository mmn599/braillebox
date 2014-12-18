#include <stdint.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "BrailleConverter.h"
#include "Solenoids.h"
#include "Timers.h"
#include "Graphics.h"
#include "Sound.h"

//Constants that define braille characters on The Grid
//{1,2,3,4,5,6} solenoids go from left to right starting at top left
const Braille numAndLetters[] = {{1,0,0,0,0,0},{1,0,1,0,0,0},{1,1,0,0,0,0},{1,1,0,1,0,0},{1,0,0,1,0,0},{1,1,1,0,0,0},{1,1,1,1,0,0}, //a,b,c,d,e,f,g/1,2,3,4,5,6,7
													 {1,0,1,1,0,0},{0,1,1,0,0,0},{0,1,1,1,0,0},{1,0,0,0,1,0},{1,0,1,0,1,0},{1,1,0,0,1,0},{1,1,0,1,1,0}, //h,i,j,k,l,m,n/8,9,0
													 {1,0,0,1,1,0},{1,1,1,0,1,0},{1,1,1,1,1,0},{1,0,1,1,1,0},{0,1,1,0,1,0},{0,1,1,1,1,0},{1,0,0,0,1,1}, //o,p,q,r,s,t,u
													 {1,0,1,0,1,1},{0,1,1,1,0,1},{1,1,0,0,1,1},{1,1,0,1,1,1},{1,0,0,1,1,1}}; //v,w,x,y,z							 
const Braille punctuation[] = {{0,0,1,0,0,0},{0,0,1,0,1,0},{0,0,1,1,0,0},{0,0,1,1,1,0},{0,0,1,1,0,1}};//,/;/:/!/.
const Braille space = {0,0,0,0,0,0};

//sempahore that indicates whether printing is happening or not
unsigned char printing = 0;

//value that keeps track of where we are in the printing of the current string
unsigned int string_index = 0;

//fixed point number representing speed in seconds
//delta is 10^-2
//aka speed is speed*10^-2 seconds
unsigned int speed = 100;

//the current braille string
BrailleString string;
	
Braille convertBraille(char value){
	//int* r; 
	if((value >= 65 && value <= 90)){
		return numAndLetters[value-('A')];
	} else if ((value >= 97 && value <= 122)){
		return numAndLetters[value-('a')];
	} else if (value == ','){
		return punctuation[0];
	} else if (value == ';'){
		return punctuation[1];
	} else if (value == ':'){
		return punctuation[2];
	} else if (value == '!'){
		return punctuation[3];
	} else if (value == '.'){
		return punctuation[4];
	} else return space;
}

BrailleString createBrailleString(char* characters, int size) {
	int i;
	BrailleString string;
	for(i=0;i<size;i++) {
		string.brailleString[i] = convertBraille(characters[i]);
		string.string[i] = characters[i];
	}
	string.size = size;
	return string;
}

void destroyBrailleString(BrailleString string) {
	free(string.brailleString);
}

void updateDots() {
	if(string_index<string.size) {
		outputSolenoids(string.brailleString[string_index].output);
		graphicsPrintCharacter(string.string[string_index],string.brailleString[string_index]);
		if(string_index!=0) {
			TIMER0_TAILR_R = speed*800000-1;  
		}
		string_index++;
	}
	else {
		Timer0A_Disable();
		outputSolenoids((int*)space.output);
		printing = 0;
		graphicsMainScreen();
		string_index = 0;
		destroyBrailleString(string);
		playAlarm(speed);
	}
}

void setSpeed(int new_speed) {
	speed = new_speed;
}

void printBraille(BrailleString input) {
	string = input;
	printing = 1;
	string_index = 0;
	updateDots();
	Timer0A_Init(updateDots,speed*800000);
}

