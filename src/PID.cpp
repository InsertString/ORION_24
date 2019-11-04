#include "main.h"
#include "PID.hpp"

PID::PID() {
  target = 0;
  error = 0;
  past_error = 0;
  integral = 0;
  derivative = 0;
  kp = 0;
  ki = 0;
  kd = 0;
}

void PID::set_PID_constants(double p, double i, double d) {

}

void PID::set_PID_variables(double target_input, double max_value, double min_value, double integral_lim) {
  target = target_input;
}

double PID::output(double current) {
  double out;

  error = target - current;


  return out;
}
