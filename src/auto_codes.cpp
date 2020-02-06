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

int scoring_timer = 0;


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
	std::string a_colour = (auto_pot.get_value() > 2047) ? "BLUE" : "RED";
  auto_colour = (auto_pot.get_value() > 2047) ? BLUE : RED;

  reset_values();
  tray_step = 0;
  auto_step = 0;
  scoring_timer = 0;

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



void auto_blue() {
  switch (auto_step) {
    // flip out tray
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
    // intake 5 (4 field, 1 preload) cubes and drive forward
    case 2 :
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_FORWARD, drive_step, 2500, 30, 0.1, a, d, 30, 6000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 6000);
    break;
    // turn towards cube 6
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
    // intake cube 6
    case 4 :
    in_take(127);
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_FORWARD, drive_step, 850, 30, 0.1, a, d, 30, 6000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 6000);
    break;
    // wait for cube six to intake
    case 5 :
    in_take(127);
    if (con1 == true) {
      stop_intake();
      auto_drive(AUTO_DRIVE_BACKWARD, drive_step, 300, 30, 0.1, a, d, 30, 1000);
    }
    con1 = (getTime(AUTO_STEP_TIMER) >= 1000);
    con2 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con3 = true;
    move_steps(con1, con2, con3, auto_step, 2000);
    break;
    // turn to set up for cubes 7-10
    case 6 :
    set_a(0, 0, 0, 0);
    set_d(0.15, 0.002, 0, 50);
    auto_turn(AUTO_TURN_L, turn_step, 730, 50, 0.01, a, d, 50, 2000);
    con1 = (turn_step == AUTO_TURN_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 2000);
    break;
    // drive back to set up for cubes 7-10
    case 7 :
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_BACKWARD, drive_step, 3200, 70, 0.1, a, d, 70, 6000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 6000);
    break;
    // turn towards cubes 7-10
    case 8 :
    stop_intake();
    set_a(0, 0, 0, 0);
    set_d(0.15, 0.002, 0, 50);
    auto_turn(AUTO_TURN_R, turn_step, 330, 50, 0.01, a, d, 50, 1000);
    con1 = (turn_step == AUTO_TURN_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 1000);
    break;
    // line up against the wall
    case 9 :
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_BACKWARD, drive_step, 500, 70, 0.1, a, d, 70, 1000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 1000);
    break;
    // intake cubes 7-10
    case 10 :
    in_take(127);
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_FORWARD, drive_step, 3350, 30, 0.1, a, d, 30, 10000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 10000);
    break;
    // drive halfway back
    case 11 :
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_BACKWARD, drive_step, 1800, 70, 0.1, a, d, 70, 10000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 10000);
    break;
    // turn to avoid a tower cube
    case 12 :
    stop_intake();
    set_a(0, 0, 0, 0);
    set_d(0.15, 0.002, 0, 50);
    auto_turn(AUTO_TURN_R, turn_step, 200, 70, 0.01, a, d, 70, 1500);
    con1 = (turn_step == AUTO_TURN_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 1500);
    break;
    // drive to avoid tower cube
    case 13 :
    stop_intake();
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_BACKWARD, drive_step, 900, 70, 0.1, a, d, 70, 10000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 10000);
    break;
    // turn into the wall
    case 14 :
    set_a(0, 0, 0, 0);
    set_d(0.15, 0.002, 0, 50);
    auto_turn(AUTO_TURN_R, turn_step, 750, 70, 0.01, a, d, 70, 3000);
    con1 = (turn_step == AUTO_TURN_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 2000);
    break;
    // drive into the scoring zone
    case 15 :
    in_take(127);
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    //auto_drive(AUTO_DRIVE_FORWARD, drive_step, 4000, 30, 0.1, a, d, 80, 10000);
    drive_along_wall(AUTO_DRIVE_FORWARD, drive_step, 4000, 30, 0.1, a, d, 80, 10000, false);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 10000);
    scoring_timer = 0;
    break;
    case 16 :
    in_take(127);
    set_a(0.02, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    //auto_drive(AUTO_DRIVE_FORWARD, drive_step, 1200, 30, 0.1, a, d, 50, 10000);
    drive_along_wall(AUTO_DRIVE_FORWARD, drive_step, 3000, 50, 0.1, a, d, 50, 10000, true);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 10000);
    scoring_timer = 0;
    break;
    case 17 :
    stop_intake();
    if (tray_limit()) {
      move_drive(0, 0);
      move_steps(auto_step, 1000);
      reset_trans_motors();
    }
    else
      move_tray(-70);
    break;
    case 18 :
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);
    score_cubes(935, 100, tray_step, scoring_timer, 0.12);
    delay(20);
    break;
  }
}



void auto_red() {
  switch (auto_step) {
    // flip out tray
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
    // intake 5 (4 field, 1 preload) cubes and drive forward
    case 2 :
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_FORWARD, drive_step, 2500, 30, 0.1, a, d, 30, 6000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 6000);
    break;
    // turn towards cube 6
    case 3 :
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);
    stop_intake();
    set_a(0,0,0,0);
    set_d(0.15, 0.002, 0, 50);
    auto_turn(AUTO_TURN_L, turn_step, 500, 40, 0.01, a, d, 50, 1000);
    con1 = (turn_step == AUTO_TURN_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 6000);
    break;
    // intake cube 6
    case 4 :
    in_take(127);
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_FORWARD, drive_step, 850, 30, 0.1, a, d, 30, 6000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 6000);
    break;
    // wait for cube six to intake
    case 5 :
    in_take(127);
    if (con1 == true) {
      stop_intake();
      auto_drive(AUTO_DRIVE_BACKWARD, drive_step, 300, 30, 0.1, a, d, 30, 1000);
    }
    con1 = (getTime(AUTO_STEP_TIMER) >= 1000);
    con2 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con3 = true;
    move_steps(con1, con2, con3, auto_step, 2000);
    break;
    // turn to set up for cubes 7-10
    case 6 :
    set_a(0, 0, 0, 0);
    set_d(0.15, 0.002, 0, 50);
    auto_turn(AUTO_TURN_R, turn_step, 800, 50, 0.01, a, d, 50, 2000);
    con1 = (turn_step == AUTO_TURN_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 2000);
    break;
    // drive back to set up for cubes 7-10
    case 7 :
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_BACKWARD, drive_step, 3200, 70, 0.1, a, d, 70, 6000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 6000);
    break;
    // turn towards cubes 7-10
    case 8 :
    stop_intake();
    set_a(0, 0, 0, 0);
    set_d(0.15, 0.002, 0, 50);
    auto_turn(AUTO_TURN_L, turn_step, 330, 50, 0.01, a, d, 50, 1000);
    con1 = (turn_step == AUTO_TURN_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 1000);
    break;
    // line up against the wall
    case 9 :
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_BACKWARD, drive_step, 500, 70, 0.1, a, d, 70, 1000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 1000);
    break;
    // intake cubes 7-10
    case 10 :
    in_take(127);
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_FORWARD, drive_step, 3350, 30, 0.1, a, d, 30, 10000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 10000);
    break;
    // drive halfway back
    case 11 :
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_BACKWARD, drive_step, 1800, 70, 0.1, a, d, 70, 10000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 10000);
    break;
    // turn to avoid a tower cube
    case 12 :
    stop_intake();
    set_a(0, 0, 0, 0);
    set_d(0.15, 0.002, 0, 50);
    auto_turn(AUTO_TURN_L, turn_step, 300, 70, 0.01, a, d, 70, 1500);
    con1 = (turn_step == AUTO_TURN_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 1500);
    break;
    // drive to avoid tower cube
    case 13 :
    stop_intake();
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    auto_drive(AUTO_DRIVE_BACKWARD, drive_step, 900, 70, 0.1, a, d, 70, 10000);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 10000);
    break;
    // turn into the wall
    case 14 :
    set_a(0, 0, 0, 0);
    set_d(0.15, 0.002, 0, 50);
    auto_turn(AUTO_TURN_L, turn_step, 800, 70, 0.01, a, d, 70, 3000);
    con1 = (turn_step == AUTO_TURN_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 2000);
    break;
    // drive into the scoring zone
    case 15 :
    in_take(127);
    set_a(0, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    //auto_drive(AUTO_DRIVE_FORWARD, drive_step, 4000, 30, 0.1, a, d, 80, 10000);
    drive_along_wall(AUTO_DRIVE_FORWARD, drive_step, 4000, 30, 0.1, a, d, 80, 10000, false);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 10000);
    scoring_timer = 0;
    break;
    case 16 :
    in_take(127);
    set_a(0.02, 0, 0, 0);
    set_d(0.2, 0.01, 0.3, 50);
    //auto_drive(AUTO_DRIVE_FORWARD, drive_step, 1200, 30, 0.1, a, d, 50, 10000);
    drive_along_wall(AUTO_DRIVE_FORWARD, drive_step, 3000, 50, 0.1, a, d, 50, 10000, true);
    con1 = (drive_step == AUTO_DRIVE_MAX_STEP);
    con2 = con3 = true;
    move_steps(con1, con2, con3, auto_step, 10000);
    scoring_timer = 0;
    break;
    case 17 :
    stop_intake();
    if (tray_limit()) {
      move_drive(0, 0);
      move_steps(auto_step, 1000);
      reset_trans_motors();
    }
    else
      move_tray(-70);
    break;
    case 18 :
    auto_arm(arm_step, 0, 0, 0, 0, 0, 0);
    score_cubes(935, 100, tray_step, scoring_timer, 0.12);
    delay(20);
    break;
  }
}




void reset_values() {
  drive_step = 0;
  turn_step = 0;
  tray_step = 0;
  con1 = false;
  con2 = false;
  con3 = false;
}
