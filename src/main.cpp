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

ADIEncoder left_enc(L_ENCODER_TOP,L_ENCODER_BOT,false);
ADIEncoder right_enc(R_ENCODER_TOP,R_ENCODER_BOT,false);

//PID objects
PID arm_pid;
PID tray_pid;


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


// move all this junk auto stuff to its own file

int auto_step = 0;
int drive_step = 0;
int turn_step = 0;
int tray_step = 0;

double a[4] = {0.1, 0, 0, 100};
double d[4] = {0.1, 0, 0, 100};

bool con1 = false;
bool con2 = false;
bool con3 = false;

void move_steps(bool condition1, bool condition2, bool condition3, int &step, int time_limit) {
	if ((condition1 == true && condition2 == true && condition3 == true) || getTime(AUTO_STEP_TIMER) >= time_limit) {
		step++;
		stop_trans();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void autonomous() {
	// stuff in here is for testing only, move to other file
	startTimer(AUTO_TIMER);
	startTimer(AUTO_STEP_TIMER);

	while (getTime(AUTO_TIMER) <= 45000) {
		switch (auto_step) {
			case 0 :
			auto_drive(AUTO_DRIVE_FORWARD, drive_step, 1000, 20, 0.02, a, d, 5000);

			con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
			con2 = true;
			con3 = true;

			move_steps(con1, con2, con3, auto_step, 5000);
			break;
			case 1 :
			break;
		}
	}
}


void opcontrol() {

	int trans_state = DRIVE;
	reset_trans_motors();
	arm.tare_position();

	int arm_state = 0;
	int scoring_step = 0;
	int scoring_timer = 0;

	int arm_timer = 0;
	int arm_delta_time = 0;
	int arm_cube_wait = 125;

	int tray_state = 0;

	std::string roller_status = "EMPTY";

	while (true) {

		arm_delta_time = millis() - arm_timer;

		if (loaded() == true) {
			roller_status = "LOADED";
		}
		else {
			roller_status = "EMPTY";
		}

		lcd::print(0, "TrayPos:[%4.0f],ArmPos:[%4.0f]", tray_position(), arm.get_position());
		lcd::print(1, "TEMPS:");
		lcd::print(2, "DriveL[F][M][B]:[%2.0f][%2.0f][%2.0f]", drive_left_F.get_temperature(), drive_left_B.get_temperature(), drive_abs_left.get_temperature());
		lcd::print(3, "DriveR[F][M][B]:[%2.0f][%2.0f][%2.0f]", drive_right_F.get_temperature(), drive_right_B.get_temperature(), drive_abs_right.get_temperature());
		lcd::print(4, "Arm:[%2.0f],Roll_L:[%2.0f],Roll_R:[%2.0f]", arm.get_temperature(), intake_left.get_temperature(), intake_right.get_temperature());
		lcd::print(5, "Roller:[%s], raw:[%4.0d]", roller_status, c_detect.get_value());


///*
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
				if (arm_delta_time > arm_cube_wait || loaded() == false) {
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
				if (arm_delta_time > arm_cube_wait || loaded() == false) {
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
		else if (master.get_digital(DIGITAL_DOWN) || master.get_digital(DIGITAL_L1)) {}
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
				score_cubes(930, 127, scoring_step, scoring_timer, 0.105);
			}
			else if (tray_state == 0 && tray_limit() == false) {
				trans_state = DRIVE;
				move_tray(-30);
				scoring_step = 0;
			}
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//*/
		delay(20);
	}
}
