/**
 * CMPUT 379 - Assignment 3
 * File Name: TimerClass.cc
 * Student Name: Jacob Bakker
 *
 * Implements basic Timer in milliseconds, allowing for a target number of milliseconds to be
 * set and for querying whether that target has been reached.
 */

#include "TimerClass.h"


/**
 * Timer Constructors
 */

Timer::Timer() {
	this->start(0);
}

Timer::Timer(int target_duration) {
	this->start(target_duration);
}

/**
 * Function: start
 * -----------------------
 * Sets the Timer's start time to the current time and saves the given
 * "target_duration".
 *
 * Parameters:
 * 	- target_duration: The target number of milliseconds.
 * Return Value: None
 * Throws: None
 */
void start(int target_duration) {
	this->start_time = HR_Clock::now();
	this->target_duration = target_duration;
}

/**
 * Function: at_target_duration
 * -----------------------
 * Returns true if "target_duration" milliseconds have passed since the
 * last time the Timer was started.
 *
 * Parameters: None
 * Return Value: True if "target_duration" has been reached, false otherwise.
 * Throws: None
 */
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
