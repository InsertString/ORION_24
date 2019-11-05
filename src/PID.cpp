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
  max = 0;
  min = 0;
  integral_limit = 0;
}

void PID::set_PID_constants(double p, double i, double d) {
  kp = p;
  ki = i;
  kd = d;
}

void PID::set_PID_variables(double target_input, double max_value, double min_value, double integral_lim) {
  target = target_input;
  max = max_value;
  min = min_value;
  integral_limit = integral_lim;
}

double PID::output(double current) {
  double out;

  past_error = error;
  error = target - current;

  if (fabs(error) < integral_limit) {
    integral += error;
  }
  else {
    integral = 0;
  }

  derivative = error - past_error;

  out = (error * kp) + (integral * ki) + (derivative * kd);

  out = out > max ? max : out < min ? min : out;

  return out;
}
