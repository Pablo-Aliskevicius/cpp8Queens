class hi_res_timer
{
	class TimerData; // forward declaration
	TimerData* m_Data;
	bool running = false;
public:
	hi_res_timer();
	~hi_res_timer();
	void Stop();
#if _MSC_VER < 1930 // or if def _MSC_VER, depending...
	using microsecs_t = long long;
#define USE_WINAPI_FOR_8QUEENS_TIMER
#else 
	using microsecs_t = double;
#endif 
	microsecs_t GetElapsedMicroseconds();
};
