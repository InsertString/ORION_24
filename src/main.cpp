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

ADILineSensor cube_detector_front(3);
//ADILineSensor cube_detector_front(4);
ADIDigitalIn cube_detector_back(4);


// classes

Cube_System cube_system;



void initialize() {
	lcd::initialize();
	intake_L.tare_position();
	intake_R.tare_position();
}




void disabled() {}




void competition_initialize() {}




void autonomous() {}




void opcontrol() {
	int left = 0;
	int right = 0;
	while (true) {
		lcd::print(0, "cube light val %f", cube_system.front_light_value());

		cube_system.driveControlCode();

		left = master.get_analog(ANALOG_LEFT_Y);
		right = -master.get_analog(ANALOG_RIGHT_Y);


		drive_left_B = left;
		drive_left_FB = left;
		drive_left_FT = -left;

		drive_right_B = right;
		drive_right_FB = right;
		drive_right_FT = -right;


		if (master.get_digital(DIGITAL_L1)) {
			elevator_L = 127;
			elevator_R = 127;
		}
		else if (master.get_digital(DIGITAL_L2)) {
			elevator_L = -127;
			elevator_R = -127;
		}
		else {
			elevator_L = 0;
			elevator_R = 0;
		}

		delay(20);
	}
}
