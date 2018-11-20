#if !defined(TIMER_CLASS_H)
#define TIMER_CLASS_H 1

#include <chrono>

class Timer {
	private:
		high_resolution_clock::time_point start_time;
		int target_duration;
	public:
		Timer();
		void start(int target_duration);
		bool at_target_duration();
}

#endif
