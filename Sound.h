/*
	A header file for the Sound class in the Alarm Clock Lab
*/

#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

void soundInit(uint16_t period, uint16_t duty);

void soundDuty(uint16_t duty);

void soundDisable();

void playAlarm(int);

#endif