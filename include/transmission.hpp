#ifndef _TRANSMISSION_HPP_
#define _TRANSMISSION_HPP_

void move_drive(int power_L, int power_R);

void move_drive_PID(double target, double speed);

void move_tray(int power);

void move_tray_PID(double target, double speed);

void reset_trans_motors();

void stop_trans();

bool tray_limit();

bool driving();

void score_cubes(double target, double speed, int &step, int &timer_1, double kp);

double tray_position();

double drive_position();

bool loaded();

#endif
