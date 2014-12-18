#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "Timers.h"


void portDInit(void) { 
  SYSCTL_RCGCGPIO_R |= 0x08;        // 1) activate port D
  while((SYSCTL_PRGPIO_R&0x08)==0){};   // allow time for clock to stabilize
	GPIO_PORTD_LOCK_R = 0x4C4F434B;
	GPIO_PORTD_CR_R = 0x80;
  GPIO_PORTD_AMSEL_R &= ~0xCF;      // 3) disable analog functionality on PD3-0
  GPIO_PORTD_PCTL_R &= ~0xFF00FFFF; // 4) GPIO
  GPIO_PORTD_DIR_R |= 0xCF;         // 5) make PD3-0 out
  GPIO_PORTD_AFSEL_R &= ~0xCF;      // 6) regular port function 
  GPIO_PORTD_DEN_R |= 0xCF;         // 7) enable digital I/O on PD3-0
}

void solenoidInit(void){
	portDInit();
}

void outputSolenoid(int which_solenoid,int on) {
	if(on) {
		GPIO_PORTD_DATA_R |= (1<<which_solenoid);
	}
	else {
		GPIO_PORTD_DATA_R &= (~(1<<which_solenoid));
	}
}

void disableAllSolenoids(){
	for(int i = 0; i<6; i++){
		int output = i;
		if(i > 3){
			output = i+2;
		}
		GPIO_PORTD_DATA_R &= (~(1<<output));
	}
}

//Takes an input of an int[6]. Each index corresponds to a solenoids
//starting from the top left of the braille going right.
//For example: [1,1,0,1,0,0] corresponds to:
// 1, 1
// 0, 1
// 0, 0
void outputSolenoids(int* nums){
	GPIO_PORTD_DATA_R &= ~0xFF;
	GPIO_PORTD_DATA_R |= nums[0] + (nums[1]<<1) + (nums[2]<<2) +
												(nums[3]<<3) + (nums[4]<<6) + (nums[5]<<7);
}

