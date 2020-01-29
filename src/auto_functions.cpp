#include "main.h"
#include "auto_functions.hpp"
#include "PID.hpp"
#include "timing.hpp"

PID Auto_Drive_PID;

void auto_drive(int direction, int &step, int target, int start_power, double precision, double accel_vars[], double deccel_vars[], int max_power, int time_limit) {

  // mamy absolute values are taken in order to account for user error

  int half_target = abs(target) / 2;
  int power_PID = 0;
  int power_drive = 0;
  int power_diff = 0;

  // pre switch-statment setup
  if (step == 0) {
    startTimer(AUTO_DRIVE_TIMEOUT);
    startTimer(AUTO_DRIVE_STEP_EXIT);
  }

  // only run the switch statement if you are not yet at the last step and you are in time
  if (step < AUTO_DRIVE_MAX_STEP && getTime(AUTO_DRIVE_TIMEOUT) <= time_limit) {
    switch (step) {
      case 0 :
      // setup
      Auto_Drive_PID.set_PID_variables((half_target * direction), (abs(max_power)), (-(abs(max_power))), accel_vars[AUTO_DRIVE_I_LIM]);
      Auto_Drive_PID.set_PID_constants(accel_vars[AUTO_DRIVE_KP], accel_vars[AUTO_DRIVE_KI], accel_vars[AUTO_DRIVE_KD]);
      reset_trans_motors();
      stop_trans();
      step++;
      startTimer(AUTO_DRIVE_STEP_EXIT);
      break;
      case 1 :
      /* acceleration code
       apply minimum power and accelerate until you reacht the halfway point
       - power_diff is the value of the max power minus the PID, if the PID outputs the max power then it will be 0
         but if its 0 the robot will not move so we power the drive with the starting power first, until the PID output
         gets smaller. when the PID output decreases, the power_diff value will increase in magnitude therfore causing
         an acceleration to occur
       - once the robot reaches the halfway point we switch the constants for the PID to the ones for decceleration
      */
      // print position data to the terminal
      std::printf("%f\n", drive_position());

      // calculate power based on output
      power_PID = Auto_Drive_PID.output(drive_position());
      power_diff = (max_power * direction) - power_PID;
      power_drive = (power_diff < start_power) ? start_power : power_diff;

      // apply power to the motors
      move_drive(power_drive, power_drive);

      // move to next step
      // if the value of the drive is more than (2 * precision)% to the halfway point or half of the time limit have passed then move to the next step
      if ((fabs(drive_position()) > (abs(half_target) - (abs(half_target) * (precision * 2)))) || (getTime(AUTO_DRIVE_STEP_EXIT) >= (time_limit / 2))) {
        step++;
        Auto_Drive_PID.set_PID_variables((abs(target) * direction), (abs(max_power)), (-(abs(max_power))), deccel_vars[AUTO_DRIVE_I_LIM]);
        Auto_Drive_PID.set_PID_constants(deccel_vars[AUTO_DRIVE_KP], deccel_vars[AUTO_DRIVE_KI], deccel_vars[AUTO_DRIVE_KD]);
        startTimer(AUTO_DRIVE_STEP_EXIT);
      }
      break;
      case 2 :
      // decelelerate
      // its a regular PID

      // print position data to the terminal
      std::printf("%f\n", drive_position());

      // set power based on output
      power_PID = Auto_Drive_PID.output(drive_position());

      // apply power to the motors
      move_drive(power_drive, power_drive);

      // move to next step
      // if the value of the drive is more than (precision)% to the halfway point or half of the time limit have passed then move to the next step
      if ((fabs(drive_position()) > (abs(target) - (abs(target) * precision))) || (getTime(AUTO_DRIVE_STEP_EXIT) >= (time_limit / 2))) {
        step++;
      }
      break;
    }
  }
  else {
    step = AUTO_DRIVE_MAX_STEP;
    stop_trans();
  }

}
