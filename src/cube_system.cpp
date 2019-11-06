#include "main.h"
#include "cube_system.hpp"



Cube_System::Cube_System() {
  stacking_step = 0;
}



// position of the elevator in encoder ticks for the specific side
double Cube_System::elevator_position(int elevator) {
  if (elevator == LEFT)
    return elevator_L.get_position();
  else if (elevator == RIGHT)
    return elevator_R.get_position();
  else
    return 0;
}



// returns whether the specific elevator is touching the limit at the top
bool Cube_System::elevator_at_limit(int elevator) {
  if (elevator == LEFT)
    return elevator_limit_L.get_value();
  else if (elevator == RIGHT)
    return elevator_limit_R.get_value();
  else
    return 0;
}



// chechs whether the specific elevator is in range of a target value
bool Cube_System::elevator_in_range(int elevator, double target, double threshold) {
  bool out = false;
  double max = target + threshold;
  double min = target - threshold;

  out = elevator_position(elevator) > min ? (elevator_position(elevator) < max ? true : false) : false;

  return out;
}




// moves each elevator to the specific position
bool Cube_System::elevators_to_position(int target) {
  bool out_L = false;
  bool out_R = false;

  // do something different depending on the target
  switch (target) {
    case TOP :
    // check whether the elevator is at the top if not, move up
    if (!elevator_at_limit(LEFT))
      elevator_L = 127;
    else {
      // move at holding power and reset the encoders
      elevator_L = 2;
      elevator_L.tare_position();
    }
    // check whether the elevator is at the top if not, move up
    if (!elevator_at_limit(RIGHT))
      elevator_R = 127;
    else {
      // move at holding power and reset the encoder
      elevator_R = 2;
      elevator_R.tare_position();
    }
    break;
    case MID :
    // move to the MID position
    elevator_L.move_absolute(MID, 200);
    elevator_R.move_absolute(MID, 200);
    // check if the elevators are in range of the MID position
    if (elevator_in_range(LEFT, MID, 20))
      out_L = true;
    // check if the elevators are in range of the MID position
    if (elevator_in_range(RIGHT, MID, 20))
      out_R = true;
    break;
    case BOT :
    // move to the BOT position
    elevator_L.move_absolute(BOT, 200);
    elevator_R.move_absolute(BOT, 200);
    // check if the elevators are in range of the MID position
    if (elevator_in_range(LEFT, BOT, 20))
      out_L = true;
      // check if the elevators are in range of the MID position
    if (elevator_in_range(RIGHT, BOT, 20))
      out_R = true;
    break;
  }
  // return whether or not both elevators have reached their target positions
  return (out_L == true && out_R == true);
}
