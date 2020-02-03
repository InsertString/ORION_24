#ifndef _AUTO_FUNCTIONS_HPP_
#define _AUTO_FUNCTIONS_HPP_

#define AUTO_DRIVE_MAX_STEP 3
#define AUTO_DRIVE_FORWARD 1
#define AUTO_DRIVE_BACKWARD -1

#define AUTO_DRIVE_KP 0
#define AUTO_DRIVE_KI 1
#define AUTO_DRIVE_KD 2
#define AUTO_DRIVE_I_LIM 3

// all values entered should be positive integers / doubles
void auto_drive(int direction, int &step, int target, int start_power, double precision, const double accel_vars[], const double deccel_vars[], int max_power, int time_limit);

#define AUTO_TURN_MAX_STEP 3
#define AUTO_TURN_L 1
#define AUTO_TURN_R -1

#define AUTO_TURN_KP 0
#define AUTO_TURN_KI 1
#define AUTO_TURN_KD 2
#define AUTO_TURN_I_LIM 3

// all values entered should be positive integer / doubles
void auto_turn(int direction, int &step, int target, int start_power, double precision, const double accel_vars[], const double deccel_vars[], int max_power, int time_limit);

#define AUTO_ARM_MAX_STEP 2

void auto_arm(int &step, int target, double kp, double ki, double kd, int i_lim, int max_power);



#endif
