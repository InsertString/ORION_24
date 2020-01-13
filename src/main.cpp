#include "main.h"

// controller definition
Controller master(E_CONTROLLER_MASTER);


// motor definition
Motor drive_left_F(DRIVE_LEFT_F);

Motor drive_left_B(DRIVE_LEFT_B);

Motor drive_right_F(DRIVE_RIGHT_F);

Motor drive_right_B(DRIVE_RIGHT_B);

Motor intake_left(INTAKE_LEFT);
Motor intake_right(INTAKE_RIGHT);

ADIDigitalIn limit(1);


void initialize() {


	lcd::initialize();

	drive_left_B.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive_left_F.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive_right_B.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive_right_F.set_brake_mode(MOTOR_BRAKE_BRAKE);

}




void disabled() {}




void competition_initialize() {}




void autonomous() {}


#define TRAY 1
#define DRIVE 0

void opcontrol() {
	int moving_state = DRIVE;
	int auto_drive_back = 0;

	drive_left_F.tare_position();
	drive_left_B.tare_position();
	drive_right_B.tare_position();
	drive_right_F.tare_position();

	while (true) {

		if (limit.get_value() == 1 && moving_state == DRIVE) {
			drive_left_F.tare_position();
			drive_left_B.tare_position();
			drive_right_B.tare_position();
			drive_right_F.tare_position();
		}

		if (master.get_digital(DIGITAL_R1)) {
			intake_left = 127;
			intake_right = -127;
		}
		else if (master.get_digital(DIGITAL_R2)) {
			intake_left = -70;
			intake_right = 70;
		}
		else {
			intake_left = 0;
			intake_right = 0;
		}

		if (moving_state == DRIVE) {
			drive_left_B = -master.get_analog(ANALOG_LEFT_Y);
			drive_left_F = master.get_analog(ANALOG_LEFT_Y);

			drive_right_B = master.get_analog(ANALOG_RIGHT_Y);
			drive_right_F = -master.get_analog(ANALOG_RIGHT_Y);
		}


		if (master.get_digital(DIGITAL_L1)) {
			drive_left_B.move_absolute(550, 30);
			drive_left_F.move_absolute(550, 30);

			drive_right_B.move_absolute(-550, 30);
			drive_right_F.move_absolute(-550, 30);
		}
		else if (master.get_digital(DIGITAL_UP)) {
			drive_left_B = 30;
			drive_left_F = 30;

			drive_right_B = -30;
			drive_right_F = -30;
		}
		else if (master.get_digital(DIGITAL_L2)) {
			drive_left_B = -50;
			drive_left_F = -50;

			drive_right_B = 50;
			drive_right_F = 50;
		}
		else if (master.get_digital(DIGITAL_DOWN)) {
			drive_left_B = 30;
			drive_left_F = -30;

			drive_right_B = -30;
			drive_right_F = 30;

			intake_left = -30;
			intake_right = 30;
		}
		else {
			moving_state = DRIVE;
			drive_left_B = 0;
			drive_left_F = 0;

			drive_right_B = 0;
			drive_right_F = 0;
		}
	}


	delay(20);
}
