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


// auto function arguments
double a[4] = {0.1, 0, 0, 100};
double d[4] = {0.1, 0, 0, 100};


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

	while (getTime(AUTO_TIMER) <= 45000) {


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
}



void auto_red() {
  switch (auto_step) {

  }
}



void auto_blue() {

}




void reset_values() {
  drive_step = 0;
  turn_step = 0;
  tray_step = 0;
}
