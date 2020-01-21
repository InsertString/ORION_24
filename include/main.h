#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_
#define PROS_USE_SIMPLE_NAMES
#define PROS_USE_LITERALS

#include "api.h"
#include "PID.hpp"
#include "transmission.hpp"
#include "intake.hpp"

using namespace pros;



// motor ports
#define DRIVE_LEFT_F 8
#define DRIVE_LEFT_B 9
#define DRIVE_RIGHT_F 7
#define DRIVE_RIGHT_B 6
#define DRIVE_ABS_L 15
#define DRIVE_ABS_R 19
#define INTAKE_LEFT 13
#define INTAKE_RIGHT 17
#define ARM 10

// sensor ports
#define TRAY_LIMIT 1


// values
#define TRAY 1
#define DRIVE 0
#define TRAY_MAX 950
#define TRAY_SLOW 700
#define TRAY_MID 500
#define TRAY_MED 300

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

extern Motor drive_left_B;
extern Motor drive_right_B;
extern Motor drive_left_F;
extern Motor drive_right_F;
extern Motor drive_abs_left;
extern Motor drive_abs_right;

extern Motor intake_left;
extern Motor intake_right;

extern ADIDigitalIn t_limit;

extern PID arm_pid;
extern PID tray_pid;

#endif  // _PROS_MAIN_H_
