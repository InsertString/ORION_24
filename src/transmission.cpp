#include "main.h"
#include "transmission.hpp"


bool tray_limit() {
  bool i = (t_limit.get_value() == 1) ? true : false;
  return i;
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
