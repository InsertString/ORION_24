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

Motor arm(ARM);

ADIDigitalIn t_limit(TRAY_LIMIT);


void initialize() {


	lcd::initialize();

	drive_left_B.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive_left_F.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive_right_B.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive_right_F.set_brake_mode(MOTOR_BRAKE_BRAKE);

	//intake_left.set_current_limit(2000);
	//intake_right.set_current_limit(2000);
}




void disabled() {}




void competition_initialize() {}




void autonomous() {}


void opcontrol() {

	int trans_state = DRIVE;
	reset_trans_motors();
	arm.tare_position();

	int arm_state = 0;
	while (true) {

		// arm
		if (master.get_digital(DIGITAL_X)) {
			arm = 127;
			arm_state = 1;
		}
		else if (master.get_digital(DIGITAL_B)) {
			arm = -127;
			arm_state = 0;
		}
		else {
			if (arm_state == 1) {
				arm = 10;
			}
			else {
				arm = 0;
			}
		}


		// rollers
		if (master.get_digital(DIGITAL_R1)) {
			in_take(127);
		}
		else if (master.get_digital(DIGITAL_R2)) {
			out_take(70);
		}
		else if (master.get_digital(DIGITAL_DOWN)) {}
		else {
			if (driving()) {
				in_take(30);
			}
			else {
				stop_intake();
			}
		}


		// drive
		if (trans_state == DRIVE) {
			move_drive(master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_Y));
		}

		if (tray_limit() && trans_state == DRIVE) {
			reset_trans_motors();
		}


		// tray
		if (master.get_digital(DIGITAL_L1)) {
			trans_state = TRAY;
			move_tray_PID(1000, 35);
		}
		else if (master.get_digital(DIGITAL_UP)) {
			trans_state = TRAY;
			move_tray(20);
		}
		else if (master.get_digital(DIGITAL_L2) && tray_limit() == false) {
			trans_state = TRAY;
			move_tray(-100);
		}
		else if (master.get_digital(DIGITAL_DOWN)) {
			trans_state = TRAY;
			move_drive(-20, -30);
			out_take(20);
		}
		else {
			trans_state = DRIVE;
		}


		delay(20);
	}
}
