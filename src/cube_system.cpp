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
  // run the intake only when not autostaking so that it doesnt interfere
  if (master.get_digital(DIGITAL_R1) && stacking_steps == 0) {
    if (cubes_in_intake() == 2) {
      intake_L.move_velocity(-25);
      intake_R.move_velocity(25);
      if (((int)intake_L.get_position() % 10)) {
        intake_L.tare_position();
        intake_R.tare_position();
      }
    }
    else {
      intake_L.move_velocity(-100);
      intake_R.move_velocity(100);
      if (((int)intake_L.get_position() % 10)) {
        intake_L.tare_position();
        intake_R.tare_position();
      }
    }

  }
  else if (master.get_digital(DIGITAL_R2)) {
    intake_L.move_velocity(50);
    intake_R.move_velocity(-50);
    if (((int)intake_L.get_position() % 10)) {
      intake_L.tare_position();
      intake_R.tare_position();
    }
  }
  else {
    intake_L.move_absolute(0, 50);
    intake_R.move_absolute(0, 50);
  }




}
