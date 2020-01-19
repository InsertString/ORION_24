#include "main.h"
#include "intake.hpp"


void in_take(int power) {
  intake_left = power;
  intake_right = -power;
}


void out_take(int power) {
  intake_left = -power;
  intake_right = power;
}


void stop_intake() {
  intake_left = 0;
  intake_right = -0;
}
