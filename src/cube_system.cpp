#include "main.h"
#include "cube_system.hpp"




Cube_System::Cube_System() {
  stacking_steps = 0;
}





void Cube_System::driveControlCode() {

  // start the autostacking process by tapping button A
  if (master.get_digital_new_press(DIGITAL_A) && cubes_in_intake() >= 2) {
    stacking_steps = 1;
  }
  // switch statment has all the steps of the stacking process
  switch (stacking_steps) {
    // move the elevators to BOT position
    case 1 :
    if (elevators_to_position(BOT))
      stacking_steps++;
    break;
    // motor the elevators to TOP position
    case 2 :
    if (elevators_to_position(TOP))
      stacking_steps = 0;
    break;
    // maintain the elevator hieght at the TOP position
    case 0 :
    elevators_to_position(TOP);
    break;
  }
<<<<<<< HEAD

  if (master.get_digital(DIGITAL_R1))
=======
  // run the intake only when not autostaking so that it doesnt interfere
  if (master.get_digital(DIGITAL_R1) && stacking_steps == 0) {
>>>>>>> 862fb78379388a33558cca41f1e324a97f75b4c3
    set_intake_power(127);
  else if (master.get_digital(DIGITAL_R2))
    set_intake_power(-127);
  else
    set_intake_power(0);


}
