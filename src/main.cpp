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



void initialize() {
	lcd::initialize();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {

	while (true) {

		pros::delay(20);
	}
}
