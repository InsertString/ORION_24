#include "main.h"
#include "transmission.hpp"


bool tray_limit() {
  bool i = (t_limit.get_value() == 1) ? true : false;
  return i;
}


bool driving() {
  if (abs(master.get_analog(ANALOG_LEFT_Y)) > 5 || abs(master.get_analog(ANALOG_RIGHT_Y)) > 5) {
    return true;
  }
  else {
    return false;
  }
}


double tray_position() {
  return fabs(drive_left_F.get_position());
}


double drive_position() {
  return -drive_abs_left.get_position();
}


bool loaded() {
  return (c_detect.get_value() > 2920) ? false : true;
}


void reset_trans_motors() {
  drive_left_F.tare_position();
	drive_left_B.tare_position();
	drive_right_B.tare_position();
	drive_right_F.tare_position();
  drive_abs_left.tare_position();
  drive_abs_right.tare_position();
}


void stop_trans() {
  drive_left_B = 0;
  drive_left_F = 0;

  drive_right_B = 0;
  drive_right_F = 0;
}


void move_drive(int power_L, int power_R) {
  drive_left_B = -power_L;
  drive_left_F = power_L;
  drive_abs_left = -power_L;

  drive_right_B = power_R;
  drive_right_F = -power_R;
  drive_abs_right = power_R;
}


void move_drive_PID(double target, double speed) {
  drive_left_B.move_absolute(-target, speed);
  drive_left_F.move_absolute(target, speed);
  drive_abs_left.move_absolute(-target, speed);

  drive_right_B.move_absolute(target, speed);
  drive_right_F.move_absolute(-target, speed);
  drive_abs_right.move_absolute(target, speed);
}


void move_tray(int power) {
  drive_left_B = power;
  drive_left_F = power;

  drive_right_B = -power;
  drive_right_F = -power;
}


void move_tray_PID(double target, double speed) {
  // 926 perfect value needed

  tray_pid.set_PID_constants(0.3, 0, 0);
  tray_pid.set_PID_variables(target, speed, -speed, 100);

  move_tray(tray_pid.output(tray_position()));
}


void score_cubes(double target, double speed, int &step, int &timer_1, double kp) {
  // variables to be updated every loop
  //int getTime(SCORE_CUBES_TIMER);
  //getTime(SCORE_CUBES_TIMER) = millis() - timer_1;
  // switch statement for autonomous scoring
  /*
  1. load a cube into the rollers
  2. pull back the cube to the scoring position
  3. move the tray to the scoring position with the PID
  4. wait for the cubes to settle
  5. outake and drive away from the stack
  */
  switch (step) {
    case 0 :
    step++;
    startTimer(SCORE_CUBES_TIMER);
    reset_trans_motors();
    break;
    case 1 :
    out_take(100);
    if (getTime(SCORE_CUBES_TIMER) > 175) {
      step++;
      in_take(10);
      startTimer(SCORE_CUBES_TIMER);
    }
    break;
    case 2 :
    // constants tuned for 11 cubes rn
    tray_pid.set_PID_constants(kp, 0.0025, 0.55);
    tray_pid.set_PID_variables(target, speed, -speed, 200);
    move_tray(tray_pid.output(tray_position()));
    if (fabs(tray_position()) < 775) {
      in_take(10);
    }
    else {
      stop_intake();
    }
    // tray never really reaches the target of the PID so end it a bit early
    if ((fabs(tray_position()) >= (target - 2)) || (getTime(SCORE_CUBES_TIMER) > 4000)) {
      step++;
      move_tray(0);
      stop_intake();
      startTimer(SCORE_CUBES_TIMER);
    }
    break;
    case 3 :
    stop_trans();
    // wait for a bit
    if (getTime(SCORE_CUBES_TIMER) > 1500) {
      step++;
      startTimer(SCORE_CUBES_TIMER);
    }
    break;
    case 4 :
    // move the tray back very slowly so that the rollers are free
    ///*
    tray_pid.set_PID_constants(0.2, 0, 0);
    tray_pid.set_PID_variables(TRAY_MED, 30, -30, 100);
    move_tray(tray_pid.output(tray_position()));
    //out_take(30);
    //*/

    if (tray_position() <= 450) {
      stop_trans();
      stop_intake();
      step++;
      startTimer(SCORE_CUBES_TIMER);
    }
    break;
    case 5 :
    out_take(30);
    //if (getTime(SCORE_CUBES_TIMER) > 200) {
    move_drive(-20, -20);
    //}
    break;
  }
}
