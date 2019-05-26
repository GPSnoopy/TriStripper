
//////////////////////////////////////////////////////////////////////
//
//                        High Resolution Timer
//
//
//  Timer returns a linear time in seconds.
//  Precision is guaranteed to be under 1 ms.
//
//
// Good practices:
//
// - Some implementation might be CPU intensive, avoid calling
//   uselessly the timer when you can store the results.
//
// - Try to clamp and/or modulus the timer's value whenever possible
//   to improve precision. (larger value -> greater loss of decimals)
//   (ex: a rotation angle can be clamped to [-360.0 ; 360.0] degrees)
//
// - Use ElapsedTime() instead of StartTime() whenever possible for
//   higher precision. (larger value -> greater loss of decimals)
//
// - Prefer short life span timers in conjunction with elapsed time.
//   (-> best precision, but be careful about CPU usage)
//
// - In last ressort uses counter values instead of time in sec
//   to compute time intervals; then use ConvertToSec().
//   Usefull when using a lot of short life span timers without
//   having make a query for all of them.
//
//////////////////////////////////////////////////////////////////////

#ifndef HEADER_GUARD_HIGH_RES_TIMER_H
#define HEADER_GUARD_HIGH_RES_TIMER_H



class high_res_timer
{
public:

	class timer_error : public base_exception 
	{
	public:
		timer_error() { }
		explicit timer_error(const std::string ErrorMsg) : base_exception(ErrorMsg) { }

		virtual std::string what() const { return std::string("high_res_timer: ") + base_exception::what(); }
	};


	typedef uint64_t counter;


	high_res_timer();	// throw(timer_error);

	// reset start time
	// (can be usefull if frequency is too high -> losing precision after a while)
	void Reset();

	counter Frequency() const;

	counter StartTimeCounter() const;
	counter ElapsedTimeCounter() const;

	// template is used to select precision
	// application decides what's the best for itself : float, double or user defined types.
	template <class T> T StartTime() const;
	template <class T> T ElapsedTime() const;

	template <class T> T ConvertToSec(counter Counter) const;
	template <class T> static T ConvertToSec(counter Counter, counter Frequency);

protected:
	counter QueryCounter() const;

private:
	counter	m_StartTime;
	counter	m_Frequency;
};





//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////

template <class T>
inline T high_res_timer::ConvertToSec(counter Counter) const
{
	return ConvertToSec<T>(Counter, Frequency());
}


template <class T>
inline T high_res_timer::ConvertToSec(counter Counter, counter Frequency)
{
	const counter NbSec = Counter / Frequency;
	const counter SubSecCounter = Counter % Frequency;

	const T tNbSec = static_cast<T>(NbSec);
	const T tSubSecCounter = static_cast<T>(SubSecCounter);
	const T tFrequency = static_cast<T>(Frequency);

	return (tNbSec + (tSubSecCounter / tFrequency));
}


inline high_res_timer::counter high_res_timer::Frequency() const {
	return m_Frequency;
}


template <class T>
inline T high_res_timer::StartTime() const {
	return ConvertToSec<T>(StartTimeCounter());
}


template <class T>
inline T high_res_timer::ElapsedTime() const {
	return ConvertToSec<T>(ElapsedTimeCounter());
}


inline high_res_timer::counter high_res_timer::StartTimeCounter() const {
	return m_StartTime;
}


inline high_res_timer::counter high_res_timer::ElapsedTimeCounter() const {
	return (QueryCounter() - StartTimeCounter());
}



#endif // HEADER_GUARD_HIGH_RES_TIMER_H
