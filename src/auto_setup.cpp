#include "main.h"
#include "auto_setup.hpp"

void move_steps(bool condition1, bool condition2, bool condition3, int &step, int time_limit) {
	if ((condition1 == true && condition2 == true && condition3 == true) || (getTime(AUTO_STEP_TIMER) >= time_limit)) {
		step++;
		reset_values();
		stop_trans();
		startTimer(AUTO_STEP_TIMER);
	}
	else {}
}
