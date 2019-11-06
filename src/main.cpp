#include "main.h"

// controller definition
Controller master(E_CONTROLLER_MASTER);

// motor definition
Motor drive_left_FT(DRIVE_LEFT_FT);
Motor drive_left_FB(DRIVE_LEFT_FB);
Motor drive_left_B(DRIVE_LEFT_B);

Motor drive_right_FT(DRIVE_RIGHT_FT);
Motor drive_right_FB(DRIVE_RIGHT_FB);
Motor drive_right_B(DRIVE_RIGHT_B);

Motor intake_L(INTAKE_L);
Motor intake_R(INTAKE_R);

Motor elevator_L(ELEVATOR_L);
Motor elevator_R(ELEVATOR_R);


// sensors

// these limits are placed at the top of each elevator
// they help keep encoder values consistant throughout the match

ADIDigitalIn elevator_limit_L(1);
ADIDigitalIn elevator_limit_R(2);

// lne sensors detect when cubes are in different parts
// of the intake for the autostack algorithum

ADILineSensor cube_detector_back(3);
ADILineSensor cube_detector_front(4);


PID drive_close_PID;
PID drive_far_PID;
PID turn_PID;
PID elevator_PID;


void initialize() {
	lcd::initialize();
}




void disabled() {}




void competition_initialize() {}




void autonomous() {}




void opcontrol() {

	while (true) {

		delay(20);
	}
}
