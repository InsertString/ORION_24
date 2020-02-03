#include "main.h"
#include "auto_functions.hpp"


PID Auto_Drive_PID;

void auto_drive(int direction, int &step, int target, int start_power, double precision, const double accel_vars[], const double deccel_vars[], int max_power, int time_limit) {

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
  else {
    if ((getTime(AUTO_DRIVE_TIMEOUT) % 1) == 0) {
      // print position data to the terminal
      std::printf("%f\n", drive_position());
    }
  }

  // only run the switch statement if you are not yet at the last step and you are in time
  if (step < AUTO_DRIVE_MAX_STEP && getTime(AUTO_DRIVE_TIMEOUT) <= time_limit) {
    switch (step) {
      case 0 :
      // setup
      Auto_Drive_PID.set_PID_variables((target * direction), (abs(max_power)), (-(abs(max_power))), accel_vars[AUTO_DRIVE_I_LIM]);
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

      // calculate power based on output
      power_PID = Auto_Drive_PID.output(drive_position());
      power_diff = (max_power * direction) - power_PID;
      power_drive = (power_diff * direction < start_power * direction) ? start_power * direction : power_diff;

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

      // set power based on output
      power_drive = Auto_Drive_PID.output(drive_position());

      // apply power to the motors
      move_drive(power_drive, power_drive);

      // move to next step
      // if the value of the drive is more than (precision)% to the halfway point or half of the time limit have passed then move to the next step
      if ( ((fabs(drive_position()) > (abs(target) - (abs(target) * precision)) && (fabs(drive_position()) < (abs(target) + (abs(target) * precision))) ) || (getTime(AUTO_DRIVE_STEP_EXIT) >= (time_limit / 2)))) {
        if (getTime(AUTO_DRIVE_EXIT) > 150) {
          step++;
          move_drive(0, 0);
        }
      }
      else {
        startTimer(AUTO_DRIVE_EXIT);
      }
      break;
    }
  }
  else {
    step = AUTO_DRIVE_MAX_STEP;
    move_drive(0, 0);
  }
}




PID Auto_Turn_PID;

void auto_turn(int direction, int &step, int raw_target, int start_power, double precision, const double accel_vars[], const double deccel_vars[], int max_power, int time_limit) {

  int target = abs(raw_target) * direction;
  int half_target = target / 2;
  int power_PID = 0;
  int power_drive = 0;
  int power_diff = 0;

  if (step == 0) {
    startTimer(AUTO_TURN_TIMEOUT);
    startTimer(AUTO_TURN_STEP_EXIT);
  }
  else {
    if ((getTime(AUTO_TURN_TIMEOUT) % 10) == 0) {
      // print position data to the terminal
      std::printf("%f\n", gyro.get_value());
    }
  }

  if (step < AUTO_TURN_MAX_STEP && getTime(AUTO_TURN_TIMEOUT) <= time_limit) {
    switch (step) {
      case 0 :
      //std::printf("setup\n");
      // setup
      Auto_Turn_PID.set_PID_variables(target, abs(max_power), -abs(max_power), accel_vars[AUTO_TURN_I_LIM]);
      Auto_Turn_PID.set_PID_constants(accel_vars[AUTO_TURN_KP], accel_vars[AUTO_TURN_KI], accel_vars[AUTO_TURN_KD]);
      reset_trans_motors();
      gyro.reset();
      stop_trans();
      startTimer(AUTO_TURN_STEP_EXIT);
      step++;
      break;
      case 1 :
      //std::printf("one\n");
      /* acceleration code
       apply minimum power and accelerate until you reacht the halfway point
       - power_diff is the value of the max power minus the PID, if the PID outputs the max power then it will be 0
         but if its 0 the robot will not move so we power the drive with the starting power first, until the PID output
         gets smaller. when the PID output decreases, the power_diff value will increase in magnitude therfore causing
         an acceleration to occur
       - once the robot reaches the halfway point we switch the constants for the PID to the ones for decceleration
      */

      // calculate power based on output
      power_PID = Auto_Turn_PID.output(gyro.get_value());
      power_diff = (max_power * direction) - (power_PID * direction);
      power_drive = ((power_diff * direction) < (start_power * direction)) ? (start_power * direction) : power_diff;

      // apply power to the motors
      move_drive(-power_drive, power_drive);

      // move to next step
      // if the value of the drive is more than (2 * precision)% to the halfway point or half of the time limit have passed then move to the next step
      if ((fabs(gyro.get_value()) > (abs(half_target) - (abs(half_target) * (precision * 10)))) || (getTime(AUTO_TURN_STEP_EXIT) >= (time_limit / 2))) {
        step++;
        Auto_Turn_PID.set_PID_variables((target), (abs(max_power)), (-(abs(max_power))), deccel_vars[AUTO_TURN_I_LIM]);
        Auto_Turn_PID.set_PID_constants(deccel_vars[AUTO_TURN_KP], deccel_vars[AUTO_TURN_KI], deccel_vars[AUTO_TURN_KD]);
        //move_drive(0, 0);
        startTimer(AUTO_TURN_STEP_EXIT);
      }
      break;
      case 2 :
      //std::printf("two\n");
      // decelelerate
      // its a regular PID

      // set power based on output
      power_drive = Auto_Turn_PID.output(gyro.get_value());

      // apply power to the motors
      move_drive(-power_drive, power_drive);

      // move to next step
      // if the value of the drive is more than (precision)% to the halfway point or half of the time limit have passed then move to the next step
      if ( ((fabs(gyro.get_value()) > (abs(target) - (abs(target) * precision))) && (fabs(gyro.get_value()) < (abs(target) + (abs(target) * precision))) ) || (getTime(AUTO_TURN_STEP_EXIT) >= (time_limit / 2))) {
        if (getTime(AUTO_TURN_EXIT) > 150) {
          step++;
          move_drive(0, 0);
        }
      }
      else {
        startTimer(AUTO_TURN_EXIT);
      }
      break;
    }
  }
  else {
    step = AUTO_TURN_MAX_STEP;
    move_drive(0, 0);
  }

}







void auto_arm(int &step, int target, double kp, double ki, double kd, int i_lim, int max_power) {
  if (target == 0) {
    if (a_limit.get_value() == 0) {
      arm = -100;
    }
    else {
      arm = 0;
      arm.tare_position();
    }
  }
  else {
    arm_pid.set_PID_constants(kp, ki, kd);
    arm_pid.set_PID_variables(target, max_power, -max_power, i_lim);

    arm = arm_pid.output(arm.get_position());
  }
}
