#include "main.h"
#include "cube_system.hpp"




Cube_System::Cube_System() {
  stacking_steps = 0;
}





void Cube_System::driveControlCode() {

  if (master.get_digital_new_press(DIGITAL_A) && cubes_in_intake() >= 2) {
    stacking_steps = 1;
  }

  switch (stacking_steps) {
    case 1 :
    if (elevators_to_position(BOT))
      stacking_steps++;
    break;
    case 2 :
    if (elevators_to_position(TOP))
      stacking_steps = 0;
    break;
    case 0 :
    elevators_to_position(TOP);
    break;
  }

  if (master.get_digital(DIGITAL_R1))
    set_intake_power(127);
  else if (master.get_digital(DIGITAL_R2))
    set_intake_power(-127);
  else
    set_intake_power(0);


}
