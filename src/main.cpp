#include "main.h"

// controller definition
Controller master(E_CONTROLLER_MASTER);


// motor definition
Motor drive_left_F(DRIVE_LEFT_F);
Motor drive_left_B(DRIVE_LEFT_B);
Motor drive_right_F(DRIVE_RIGHT_F);
Motor drive_right_B(DRIVE_RIGHT_B);

Motor drive_abs_left(DRIVE_ABS_L);
Motor drive_abs_right(DRIVE_ABS_R);

Motor intake_left(INTAKE_LEFT);
Motor intake_right(INTAKE_RIGHT);

Motor arm(ARM);

ADIDigitalIn t_limit(TRAY_LIMIT);
ADIDigitalIn a_limit(A_LIMIT);

ADILineSensor c_detect(CUBE_DETECTER);

ADIPotentiometer auto_pot(AUTO_POT);
ADIGyro gyro(GYRO);

ADIDigitalIn barriar_limit(BARRIAR_LIMIT);

//PID objects
PID arm_pid;
PID tray_pid;

int auto_running = 0;
int scoring_step = 0;

std::string roller_status = "EMPTY";
std::string a_colour = (auto_pot.get_value() > 2047) ? "BLUE" : "RED";

void initialize() {


	lcd::initialize();

	drive_left_B.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive_left_F.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive_right_B.set_brake_mode(MOTOR_BRAKE_BRAKE);
	drive_right_F.set_brake_mode(MOTOR_BRAKE_BRAKE);

	//intake_left.set_current_limit(2000);
	//intake_right.set_current_limit(2000);

	gyro.reset();
}




void disabled() {}




void competition_initialize() {}


// move all this junk auto stuff to its own file


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void autonomous() {
	auto_loop();
}

void opcontrol() {

	int trans_state = DRIVE;
	reset_trans_motors();
	arm.tare_position();

	int arm_state = 0;

	int scoring_timer = 0;

	int arm_timer = 0;
	int arm_delta_time = 0;
	int arm_cube_wait = 200;

	int tray_state = 0;



	while (true) {

		auto_colour = (auto_pot.get_value() > 2047) ? BLUE : RED;

		arm_delta_time = millis() - arm_timer;

		lcd_function();




		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		if (master.get_digital_new_press(DIGITAL_B))
			auto_running = 1;
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!
		// ONLY FOR TESTING!!!! REMOVE BEFORE COMP!!!





///*

		if (auto_running == 0) {

			// arm ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			if (a_limit.get_value() == 1) {
				arm.tare_position();
			}

			if (master.get_digital_new_press(DIGITAL_L1)) {
				if (arm_state == 1) {
					arm_state = 2;
				}
				else if (arm_state != 1) {
					arm_state = 1;
					arm_timer = millis();
				}
			}
			else if (master.get_digital_new_press(DIGITAL_L2)) {
				arm_state = 0;
				arm_timer = millis();
			}
			else if (master.get_digital(DIGITAL_A)) {
				arm = 127;
				arm_state = 0;
			}
			else {
				if (arm_state == 1) {
					if (arm_delta_time > arm_cube_wait || loaded() == true) {
						arm_pid.set_PID_constants(0.6, 0, 0);
						arm_pid.set_PID_variables(1250, 127, -127, 100);
						arm = arm_pid.output(arm.get_position());
						if (master.get_digital(DIGITAL_R2)) {
							out_take(127);
						}
						else {
							stop_intake();
						}
					}
					else {
						out_take(127);
					}
				}
				else if (arm_state == 2) {
					if (arm_delta_time > arm_cube_wait || loaded() == true) {
						arm_pid.set_PID_constants(0.6, 0, 0);
						arm_pid.set_PID_variables(1875, 127, -127, 100);
						arm = arm_pid.output(arm.get_position());
						if (master.get_digital(DIGITAL_R2)) {
							out_take(127);
						}
						else {
							stop_intake();
						}
					}
					else {
						out_take(127);
					}
				}
				else if (arm_state == 0) {
					if (a_limit.get_value() == 0) {
						arm = -100;
					}
					else {
						arm = 0;
					}
				}
			}
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//



			// rollers ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			if (master.get_digital(DIGITAL_R1)) {
				in_take(127);
			}
			else if (master.get_digital(DIGITAL_R2)) {
				if (arm_state == 0) {
					out_take(70);
				}
			}
			else if (master.get_digital(DIGITAL_DOWN) || master.get_digital(DIGITAL_L1) || arm_state == 1) {}
			else if (arm_state == 0) {
				stop_intake();
			}
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//



			// drive ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

			if (trans_state == DRIVE) {
				move_drive(master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_Y));
			}

			if (tray_limit() && trans_state == DRIVE) {
				reset_trans_motors();
			}
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//



			// tray ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			if (master.get_digital_new_press(DIGITAL_UP)) {
				tray_state = 1;
				trans_state = TRAY;
				//score_cubes(960, 127, scoring_step, scoring_timer);
			}
			else if (master.get_digital(DIGITAL_LEFT)) {
				trans_state = TRAY;
				move_tray(30);
			}
			else if (master.get_digital_new_press(DIGITAL_DOWN)) {
				tray_state = 0;
				trans_state = TRAY;
				//move_tray(-60);
			}
			else if (master.get_digital(DIGITAL_RIGHT)) {
				trans_state = TRAY;
				move_drive(-20, -30);
				out_take(70);
			}
			else {
				if (tray_state == 0 && tray_limit() == true) {
					trans_state = DRIVE;
					scoring_step = 0;
					scoring_timer = 0;
				}
				else if (tray_state == 1) {
					trans_state = TRAY;
					// current values score 11 cubes extremely consistently
					score_cubes(945, 100, scoring_step, scoring_timer, 0.12);
				}
				else if (tray_state == 0 && tray_limit() == false) {
					trans_state = DRIVE;
					move_tray(-30);
					scoring_step = 0;
				}
			}
		}
		else if (auto_running == 1) {
			auto_loop();
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//*/
		delay(20);
	}
}
