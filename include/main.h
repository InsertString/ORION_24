#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_
#define PROS_USE_SIMPLE_NAMES
#define PROS_USE_LITERALS

#include "api.h"
#include "PID.hpp"
#include "transmission.hpp"

using namespace pros;



// motor ports
#define DRIVE_LEFT_F 2
#define DRIVE_LEFT_B 12
#define DRIVE_RIGHT_F 1
#define DRIVE_RIGHT_B 11
#define INTAKE_LEFT 4
#define INTAKE_RIGHT 6

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

extern Motor drive_left_B;
extern Motor drive_right_B;
extern Motor drive_left_F;
extern Motor drive_right_F;

extern ADIDigitalIn t_limit;

#endif  // _PROS_MAIN_H_
