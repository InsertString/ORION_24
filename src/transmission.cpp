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

  drive_right_B = power_R;
  drive_right_F = -power_R;
}


void move_drive_PID(double target, double speed) {
  drive_left_B.move_absolute(-target, speed);
  drive_left_F.move_absolute(target, speed);

  drive_right_B.move_absolute(target, speed);
  drive_right_F.move_absolute(-target, speed);
}


void move_tray(int power) {
  drive_left_B = power;
  drive_left_F = power;

  drive_right_B = -power;
  drive_right_F = -power;
}


void move_tray_PID(double target, double speed) {
  drive_left_B.move_absolute(target, speed);
  drive_left_F.move_absolute(target, speed);

  drive_right_B.move_absolute(-target, speed);
  drive_right_F.move_absolute(-target, speed);
}


void score_cubes(int &step, int &timer_1) {
  int delta_time;

  switch(step) {
    // ensure that the tray starts from the resting position
    case 0 :
    if (tray_limit() == true) {
      move_tray(0);
      reset_trans_motors();
      step++;
    }
    else
      move_tray(-10);
    break;

    // move the tray at max power to the medium speed position
    case 1 :
    if (tray_position() < TRAY_MED) {
      move_tray(127);
      in_take(10);
    }
    else
      step++;
    break;

    // switch to slow PID between medium and slow position
    case 2 :
    if (tray_position() < TRAY_SLOW)
      move_tray_PID(TRAY_SLOW + 20, 20);
    else
      step++;
    break;

    // move tray at etremely slow speed to keep stack straight
    case 3 :
    if (tray_position() < TRAY_MAX)
      move_tray(10);
    else
      step++;
    break;

    // stop at max tray position
    case 4 :
    stop_trans();
    timer_1 = millis();
    step++;
    break;

    // wait for half a second
    case 5 :
    delta_time = millis() - timer_1;
    if (delta_time > 500)
      step++;
    break;

    // move tray back a bit so that it doesnt interfier with the intake
    case 6:
    if (tray_position() > 800)
      move_tray(-20);
    else {
      stop_trans();
      step++;
    }
    break;

    // out-take and drive back at the same time for 750ms
    case 7 :
    delta_time = millis() - timer_1;
    out_take(10);
    move_drive(-10, -10);
    if (delta_time > 750) {
      stop_trans();
      stop_intake();
    }
    break;
  }
}
