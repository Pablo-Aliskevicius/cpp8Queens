<<<<<<< HEAD
#ifdef USE_WINAPI_FOR_8QUEENS_TIMER
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
	microsecs_t GetElapsedMicroseconds() const
	{
		return microsecs_t(ElapsedMicroseconds.QuadPart);
	}

};
#else
#include <chrono>
#include "high_res_clock.h"

class hi_res_timer::TimerData
{
	using Clock = std::chrono::high_resolution_clock;
	const Clock::time_point m_begin = Clock::now();
	microsecs_t m_duration = microsecs_t(-1);
public:
	TimerData() = default;
	void Stop()
	{
		const auto end_ = Clock::now();
		using std::chrono::nanoseconds;
		using std::chrono::duration_cast;
		auto ns = duration_cast<nanoseconds>(end_ - m_begin);
		m_duration = microsecs_t(double(ns.count()) / 1000.0);
	}
	microsecs_t GetElapsedMicroseconds() const
	{
		return microsecs_t(m_duration);
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

hi_res_timer::microsecs_t hi_res_timer::GetElapsedMicroseconds()
{
	if (running)
	{
		Stop();
	}
	return m_Data->GetElapsedMicroseconds();
}
=======
#ifdef USE_WINAPI_FOR_8QUEENS_TIMER
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
	microsecs_t GetElapsedMicroseconds() const
	{
		return microsecs_t(ElapsedMicroseconds.QuadPart);
	}

};
#else
#include <chrono>
#include "high_res_clock.h"

class hi_res_timer::TimerData
{
	using Clock = std::chrono::high_resolution_clock;
	const Clock::time_point m_begin = Clock::now();
	microsecs_t m_duration = (microsecs_t)  - 1;
public:
	TimerData() = default;
	void Stop()
	{
		const auto end_ = Clock::now();
		using std::chrono::nanoseconds;
		using std::chrono::duration_cast;
		auto ns = duration_cast<nanoseconds>(end_ - m_begin);
		m_duration = microsecs_t(double(ns.count()) / 1000.0);
	}
	microsecs_t GetElapsedMicroseconds() const
	{
		return microsecs_t(m_duration);
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

hi_res_timer::microsecs_t hi_res_timer::GetElapsedMicroseconds()
{
	if (running)
	{
		Stop();
	}
	return m_Data->GetElapsedMicroseconds();
}
>>>>>>> 6ab5f20b8a20e21c5359590c5a2ca592c1d23f98
