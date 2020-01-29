#ifndef _AUTO_FUNCTIONS_HPP_
#define _AUTO_FUNCTIONS_HPP_

#define AUTO_DRIVE_MAX_STEP 10
#define AUTO_DRIVE_FORWARD 1
#define AUTO_DRIVE_BACKWARD -1

#define AUTO_DRIVE_KP 0
#define AUTO_DRIVE_KI 1
#define AUTO_DRIVE_KD 2
#define AUTO_DRIVE_I_LIM 3

// all values entered should be possitive integers / doubles
void auto_drive(int direction, int &step, int target, int start_power, const double precision, const double accel_vars[], double deccel_vars[], int time_limit);

#endif
