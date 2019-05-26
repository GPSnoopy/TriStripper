
#include "stdafx.h"
#include "high_res_timer.h"

#if defined (WIN32)

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#undef max
#undef min




high_res_timer::high_res_timer()
{
	// get frequency
	if (QueryPerformanceFrequency((LARGE_INTEGER *) &m_Frequency) == 0)
		throw timer_error("QueryPerformanceFrequency() not supported");

	// Reset()
	if (QueryPerformanceCounter((LARGE_INTEGER *) &m_StartTime) == 0)
		throw timer_error("QueryPerformanceCounter() not supported");
}



high_res_timer::counter high_res_timer::QueryCounter() const
{
	counter Counter;

	QueryPerformanceCounter((LARGE_INTEGER *) &Counter);

	return Counter;
}

#endif // defined (WIN32)




#if defined (UNIX)

#include <sys/time.h>



high_res_timer::high_res_timer()
{
	timeval tv;

	// test routine
	if (gettimeofday(&tv, NULL))
		throw timer_error("gettimeofday() error");

	// get frequency (gettimeofday() can get down to 1 micro-second)
	m_Frequency = 1000000;
	m_StartTime = (counter(tv.tv_sec) * m_Frequency + counter(tv.tv_usec));
}



high_res_timer::counter high_res_timer::QueryCounter() const
{
	timeval tv;
	gettimeofday(&tv, NULL);

	return (counter(tv.tv_sec) * m_Frequency + counter(tv.tv_usec));
}



#endif // defined (UNIX)



void high_res_timer::Reset()
{
	m_StartTime = QueryCounter();
}


