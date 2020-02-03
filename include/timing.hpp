#ifndef _TIMING_HPP_
#define _TIMING_HPP_


#define AUTO_TIMER 0
#define AUTO_STEP_TIMER 1

#define AUTO_DRIVE_TIMEOUT 2
#define AUTO_DRIVE_STEP_EXIT 3
#define AUTO_DRIVE_EXIT 4

#define AUTO_TURN_TIMEOUT 5
#define AUTO_TURN_STEP_EXIT 6
#define AUTO_TURN_EXIT 7

#define AUTO_ARM_TIMEOUT 8
#define AUTO_ARM_STEP_EXIT 9


void startTimer(int slot);
int getTime(int slot);

#endif
