#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_
#define PROS_USE_SIMPLE_NAMES
#define PROS_USE_LITERALS

#include "api.h"
#include "PID.hpp"
#include "cube_system.hpp"

using namespace pros;



// motor ports
#define DRIVE_LEFT_FT 1
#define DRIVE_LEFT_FB 2
#define DRIVE_LEFT_B 3
#define DRIVE_RIGHT_FT 4
#define DRIVE_RIGHT_FB 5
#define DRIVE_RIGHT_B 6
#define INTAKE_L 7
#define INTAKE_R 8
#define ELEVATOR_L 9
#define ELEVATOR_R 10

// sensor ports




#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#endif

extern Controller master;

extern Motor drive_left_FT;
extern Motor drive_left_FB;
extern Motor drive_left_B;

extern Motor drive_right_FT;
extern Motor drive_right_FB;
extern Motor drive_right_B;

extern Motor intake_L;
extern Motor intake_R;

extern Motor elevator_L;
extern Motor elevator_R;

extern ADIDigitalIn elevator_limit_L;
extern ADIDigitalIn elevator_limit_R;

extern ADILineSensor cube_detector_front;
//extern ADILineSensor cube_detector_front;
extern ADIDigitalIn cube_detector_back;

#endif  // _PROS_MAIN_H_
