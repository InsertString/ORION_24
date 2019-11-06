#include "main.h"
#include "cube_system.hpp"


// sets the power for the intake motors
void Cube_System::set_intake_power(int power) {
  intake_L = power;
  intake_R = -power;
}



double Cube_System::front_light_value() {
  return cube_detector_front.get_value();
}




double Cube_System::back_light_value() {
  return cube_detector_back.get_value();
}





int Cube_System::cubes_in_intake() {
  int count = 0;
  // if a cube is in front of a light sensor, then increase the cube count
  if (back_light_value() > LIGHT_THRESHOLD)
    count++;
  if (front_light_value() > LIGHT_THRESHOLD)
    count++;
  return count;
}
