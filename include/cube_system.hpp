#ifndef _CUBE_SYSTEM_HPP_
#define _CUBE_SYSTEM_HPP_




class Cube_System {
public:
  Cube_System();

  #define OFF 0
  #define LEFT 1
  #define RIGHT 2
  #define TOP 0
  #define MID 100
  #define BOT 200
  #define LIGHT_THRESHOLD 500

  // elevator functions
  double elevator_position(int elevator);
  bool elevator_at_limit(int elevator);
  bool elevator_in_range(int elevator, double target, double threshold);
  bool elevators_to_position(int target);
  // intake functions
  void set_intake_power(int power);
  int cubes_in_intake();
  double front_light_value();
  double back_light_value();
  // system functions
  int stacking_steps;
  int stack_cubes();
  void driveControlCode();
};


#endif
