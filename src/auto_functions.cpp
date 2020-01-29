#include "main.h"
#include "auto_functions.hpp"
#include "PID.hpp"

PID Auto_Drive_PID;

void auto_drive(int direction, int &step, int target, int start_power, double max_power, double kp, double ki, double kd, double i_limit, int time_limit) {

  int half_target = target / 2;

  switch (step) {
    case 0 :
    // setup
    Auto_Drive_PID.set_PID_variables(half_target, max_power, start_power, i_limit);
    Auto_Drive_PID.set_PID_constants(kp, ki, kd);
    reset_trans_motors();
    stop_trans();
    step++;
    break;
    case 1 :
    // accelerate

    break;
    case 2 :
    // decelelerate
    break;
  }

}
