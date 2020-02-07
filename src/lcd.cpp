#include "main.h"
#include "lcd.hpp"

void lcd_function() {
  a_colour = (auto_pot.get_value() > 2047) ? "BLUE" : "RED";

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
}
