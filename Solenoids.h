/*
	A header file for the solenoids which are connected to Port D
*/


#ifndef SOLENOIDS_H
#define SOLENOIDS_H

/*
 * Initializes port D, which controls the solenoids.
*/
void solenoidInit(void);

/*
 * Outputs to a specified solenoid either a one or a zero
 * depending on "on".
*/
void outputSolenoid(int which_solenoid,int on);

/*
 * Outputs to all 6 solenoids their specified value in nums.
*/
void outputSolenoids(int* nums);

#endif