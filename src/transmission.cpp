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


void reset_trans_motors() {
  drive_left_F.tare_position();
	drive_left_B.tare_position();
	drive_right_B.tare_position();
	drive_right_F.tare_position();
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

  tray_pid.set_PID_constants(0.1, 0, 0);
  tray_pid.set_PID_variables(target, speed, -speed, 100);

  move_tray(tray_pid.output(tray_position()));
}


void score_cubes(double target, double speed, int &step, int &timer_1) {
  int delta_time;
  delta_time = millis() - timer_1;
  switch (step) {
    case 0 :
    tray_pid.set_PID_constants(0.1, 0, 0);
    tray_pid.set_PID_variables(target, speed, -speed, 100);
    move_tray(tray_pid.output(tray_position()));

    if (tray_position() <= 500 && (millis() % 50) == 0) {
      in_take(20);
      timer_1 = millis();
    }
    else if (tray_position() <= 500 && delta_time > 200) {
      stop_intake();
    }
    else {
      stop_intake();
    }

    if (tray_position() >= 920) {
      step++;
      stop_trans();
      timer_1 = millis();
    }
    break;
    case 1 :
    if (delta_time > 750) {
      step++;
      timer_1 = millis();
    }
    break;
    case 2 :
    tray_pid.set_PID_constants(0.1, 0, 0);
    tray_pid.set_PID_variables(TRAY_MED, speed, -speed, 100);
    move_tray(tray_pid.output(tray_position()));

    if (tray_position() <= 550) {
      stop_trans();
      step++;
      timer_1 = millis();
    }
    break;
    case 3 :
    out_take(70);
    if (delta_time > 200) {
      move_drive(-20, -20);
    }
    break;
  }
}
