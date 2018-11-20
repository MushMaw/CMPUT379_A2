#include "TimerClass.h"

Timer::Timer() {
	this->start(0);
}

void start(int target_duration) {
	this->start_time = HR_Clock::now();
	this->target_duration = 0;
}

bool at_target_duration() {
	HR_Clock end_time;
	millisec ms_dur;
	duration raw_dur;

        end_time = HR_Clock::now();
	raw_dur = end_time - this->start_time;
	ms_dur = std::chrono::duration_cast<millisec>(raw_dur);

	if (ms_dur > this->target_duration) { return true; }
	else { return false; }
}
