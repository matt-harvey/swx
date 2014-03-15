// Copyright 2014 Matthew Harvey

#ifndef GUARD_interval_hpp_6955864893717628
#define GUARD_interval_hpp_6955864893717628

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>

namespace swx
{

/**
 * Represent an interval of time. It may be open (still running) or
 * closed (has an end).
 */
class Interval
{
// special member functions
public:

	explicit Interval(boost::posix_time::ptime const& p_begin);

	Interval(Interval const& rhs) = default;
	Interval(Interval&& rhs) = default;
	Interval& operator=(Interval const& rhs) = default;
	Interval& operator=(Interval&& rhs) = default;
	~Interval() = default;

// ordinary member functions
public:
	
	void set_end(boost::posix_time::ptime const& p_end);

// member variables
private:
	boost::posix_time::ptime m_begin;
	boost::optional<boost::posix_time::ptime> m_end;


};  // class Interval

}  // namespace swx

#endif  // GUARD_interval_hpp_6955864893717628
