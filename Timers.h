#ifndef TIMER0A_H
#define TIMER0A_H

#include <stdint.h>

void Timer0A_Init(void(*task)(void), uint32_t period);
void Timer0A_Disable(void);

void Timer1A_Init(void(*task)(void), uint32_t period);
void Timer1A_Disable(void);


#endif