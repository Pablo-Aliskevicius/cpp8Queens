#ifdef _MSC_VER // or _MSC_VER < 1930, depending...

/*

From https://docs.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps#examples-for-acquiring-time-stamps
Unfortunately, std::chrono::high_resolution_clock is low-resolution for some older versions of Visual C++.

A standards-based implementation is available below for reference.

*/

#include <windows.h>
#include <profileapi.h>

#include "high_res_clock.h"

class hi_res_timer::TimerData
{
	LARGE_INTEGER StartingTime;
	LARGE_INTEGER Frequency;
	LARGE_INTEGER EndingTime;
	LARGE_INTEGER ElapsedMicroseconds;
public:
	TimerData()
	{
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartingTime);
	}
	~TimerData() = default;
	void Stop()
	{
		QueryPerformanceCounter(&EndingTime);
		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
		//
		// We now have the elapsed number of ticks, along with the
		// number of ticks-per-second. We use these values
		// to convert to the number of elapsed microseconds.
		// To guard against loss-of-precision, we convert
		// to microseconds *before* dividing by ticks-per-second.
		//

		ElapsedMicroseconds.QuadPart *= 1000000;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
	}
	long long GetElapsedMicroseconds() const
	{
		return ElapsedMicroseconds.QuadPart;
	}

};
#else
#include <chrono>
#include "high_res_clock.h"

class hi_res_timer::TimerData
{
	using Clock = std::chrono::high_resolution_clock;
	const Clock::time_point m_begin = Clock::now();
	long long m_duration = -1LL;
public:
	TimerData() = default;
	void Stop()
	{
		const auto end_ = Clock::now();
		using std::chrono::microseconds;
		using std::chrono::duration_cast;
		auto ns = duration_cast<microseconds>(end_ - m_begin);
		m_duration = ns.count();
	}
	long long GetElapsedMicroseconds() const
	{
		return m_duration;
	}
};

#endif // _MSC_VER

hi_res_timer::hi_res_timer()
{
	m_Data = new TimerData;
	running = true;
}

hi_res_timer::~hi_res_timer()
{
	delete m_Data;
}

void hi_res_timer::Stop()
{
	m_Data->Stop();
	running = false;
}

long long hi_res_timer::GetElapsedMicroseconds()
{
	if (running)
	{
		Stop();
	}
	return m_Data->GetElapsedMicroseconds();
}
