#if !defined(TIMER_CLASS_H)
#define TIMER_CLASS_H 1

#include <chrono>

typedef std::chrono::high_resolution_clock HR_Clock;
typedef std::chrono::milliseconds millisec;
typedef std::chrono::duration<int> duration;

class Timer {
	private:
		std::chrono::high_resolution_clock::time_point start_time;
		int target_duration;
	public:
		Timer();
		void start(int target_duration);
		bool at_target_duration();
};

#endif
