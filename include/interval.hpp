// Copyright 2014 Matthew Harvey

#ifndef GUARD_interval_hpp_6955864893717628
#define GUARD_interval_hpp_6955864893717628

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
#include <jewel/exception.hpp>

namespace swx
{

JEWEL_DERIVED_EXCEPTION(IntervalException, jewel::Exception);

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
	
	/**
	 * Close the interval at p_end.
	 *
	 * @throws IntervalException if already closed.
	 */
	void close
	(	boost::posix_time::ptime const& p_end =
			boost::posix_time::second_clock::local_time()
	);

	bool is_open() const;
	bool is_closed() const;

// member variables
private:
	boost::posix_time::ptime m_begin;
	boost::optional<boost::posix_time::ptime> m_end;


};  // class Interval

}  // namespace swx

#endif  // GUARD_interval_hpp_6955864893717628
