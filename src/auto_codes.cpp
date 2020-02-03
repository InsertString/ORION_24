#include "main.h"
#include "auto_codes.hpp"

// auto variables~~~~~~~~~~~~~~~~~~~~~~~//

// colour
int auto_colour = 0;

// step variables
int auto_step = 0;
int drive_step = 0;
int turn_step = 0;
int tray_step = 0;
int arm_step = 0;


// auto function arguments
double a[4] = {0.01,0,0,0};//{0.002, 0, 0, 100};
double d[4] = {0.1,0.01,0.1,100};//{0.2, 0.01, 0, 50};

void set_a(double p, double i, double d, int il) {
  a[0] = p;
  a[1] = i;
  a[2] = d;
  a[3] = il;
}


void set_d(double p, double i, double dd, int il) {
  d[0] = p;
  d[1] = i;
  d[2] = dd;
  d[3] = il;
}

// next step conditions
bool con1 = false;
bool con2 = false;
bool con3 = false;



void auto_selection() {
  if (auto_colour == RED)
    auto_red();
  else if (auto_colour == BLUE) {
    auto_blue();
  }
}



void auto_loop() {
  startTimer(AUTO_TIMER);
	startTimer(AUTO_STEP_TIMER);

  std::string roller_status = "EMPTY";
	std::string a_colour = (auto_pot.get_value() > 2047) ? "RED" : "BLUE";
  auto_colour = (auto_pot.get_value() > 2047) ? RED : BLUE;

  reset_values();
  auto_step = 0;

	while (getTime(AUTO_TIMER) <= 45000) {

    if (master.get_digital_new_press(DIGITAL_B)) {
      auto_running = 0;
      break;
    }

    if (loaded() == true)
			roller_status = "LOADED";
		else
			roller_status = "EMPTY";

		lcd::print(0, "TrayPos:[%4.0f],ArmPos:[%4.0f]", tray_position(), arm.get_position());
    lcd::print(1, "Auto Colour : [%s]", a_colour);
		lcd::print(2, "TEMPS:");
		lcd::print(3, "DriveL[F][M][B]:[%2.0f][%2.0f][%2.0f]", drive_left_F.get_temperature(), drive_left_B.get_temperature(), drive_abs_left.get_temperature());
		lcd::print(4, "DriveR[F][M][B]:[%2.0f][%2.0f][%2.0f]", drive_right_F.get_temperature(), drive_right_B.get_temperature(), drive_abs_right.get_temperature());
		lcd::print(5, "Arm:[%2.0f],Roll_L:[%2.0f],Roll_R:[%2.0f]", arm.get_temperature(), intake_left.get_temperature(), intake_right.get_temperature());
		lcd::print(6, "Roller:[%s], raw:[%4.0d]", roller_status, c_detect.get_value());


		auto_selection();
	}

  stop_trans();
  stop_intake();
  auto_running = 0;
}



void auto_red() {
  switch (auto_step) {
    case 0 :
    auto_arm(arm_step, 1875, 0.6, 0, 0, 0, 127);

    if (arm.get_position() > 1600) {
      out_take(127);
    }

    con1 = (arm.get_position() > 1700);
    con2 = true;
    con3 = true;

    //if (arm.get_position() > 1000) {
    //  auto_step++;
    //}

    move_steps(con1, con2, con3, auto_step, 1000);
    break;
    case 1 :
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);

    if (arm.get_position() < 500) {
      con1 = true;
      in_take(127);
    }

    con2 = arm.get_position() == 0;
    con3 = true;

    move_steps(con1, con2, con3, auto_step, 2000);
    break;
    case 2 :
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_FORWARD, drive_step, 2500, 30, 0.1, a, d, 30, 6000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 6000);
    break;
    case 3 :
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);
    stop_intake();
    set_a(0,0,0,0);
    set_d(0.15, 0.002, 0, 50);
    auto_turn(AUTO_TURN_R, turn_step, 330, 40, 0.01, a, d, 50, 1000);
    con1 = (turn_step == AUTO_TURN_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 6000);
    break;
    case 4 :
    in_take(127);
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_FORWARD, drive_step, 750, 30, 0.1, a, d, 30, 6000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 6000);
    break;
  }
}



void auto_blue() {

}




void reset_values() {
  drive_step = 0;
  turn_step = 0;
  tray_step = 0;
  con1 = false;
  con2 = false;
  con3 = false;
}
