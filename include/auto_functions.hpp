#ifndef _AUTO_FUNCTIONS_HPP_
#define _AUTO_FUNCTIONS_HPP_

#define AUTO_DRIVE_MAX_STEP 10
#define AUTO_DRIVE_FORWARD 1
#define AUTO_DRIVE_BACKWARD 2

void auto_drive(int direction, int &step, int target, int start_power, double max_power, double kp, double ki, double kd, double i_limit, int time_limit);

#endif
