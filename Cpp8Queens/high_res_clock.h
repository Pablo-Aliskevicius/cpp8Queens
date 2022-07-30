#include <memory>

// From https://docs.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps#examples-for-acquiring-time-stamps
// Unfortunately, std::chrono::high_resolution_clock is low-resolution for some versions of Visual C++.
class hi_res_timer
{
	class TimerData; // forward declaration
	TimerData* m_Data;
	bool running = false;
public:
	hi_res_timer();
	~hi_res_timer();
	void Stop();
	long long GetElapsedMicroseconds();
};





