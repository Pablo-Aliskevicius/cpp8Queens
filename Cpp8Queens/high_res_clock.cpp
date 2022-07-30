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
	long long GetElapsedMicroseconds()
	{
		return ElapsedMicroseconds.QuadPart;
	}

};

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