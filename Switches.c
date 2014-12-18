/*
	This class details the drivers for the 4 buttons used in this lab.
	Authors: Kapil Gowru and Matthew Normyle
*/
// momentary, positive logic button connected to PE4 (trigger on rising edge)
// momentary, positive logic button connected to PE5 (trigger on rising edge)

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#define NVIC_EN0_INT4           0x00000010  // Interrupt 4 enable


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void (*PlayMessageTask)(void);

void switchInit(void(*playMessageTask)(void)) {
	PlayMessageTask = playMessageTask;
  SYSCTL_RCGCGPIO_R |= 0x00000020; // activate port B
	//GPIO_PORTB_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_DIR_R &= ~0x10;  // make PE5-4 in 
  GPIO_PORTF_AMSEL_R &= ~0x10;// disable analog function on PE5-4
  GPIO_PORTF_PCTL_R &= ~0x000F0000; // configure PE5-4 as GPIO 
  GPIO_PORTF_AFSEL_R &= ~0x10;// disable alt funct on PE5-4 
  GPIO_PORTF_DEN_R |= 0x10;   // enable digital I/O on PE5-4
  GPIO_PORTF_IS_R &= ~0x10;   // PE5-4 is edge-sensitive 
  GPIO_PORTF_IBE_R &= ~0x10;  // PE5-4 is not both edges 
  GPIO_PORTF_IEV_R |= 0x10;  // PE5-4 rising edge event
  GPIO_PORTF_ICR_R = 0x10;    // clear flag5-4
  GPIO_PORTF_IM_R |= 0x10;    // enable interrupt on PE5-4
                             // GPIO PortE=priority 2
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF0FFFFF)|0x00400000; // bits 5-7
  NVIC_EN0_R = NVIC_EN0_INT30; // enable interrupt 4 in NVIC
}

void GPIOPortF_Handler(void){
  if(GPIO_PORTF_RIS_R&0x10){  // poll PB3
    GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag
		SysTick_Init(800000,1,PlayMessageTask);
  }
}