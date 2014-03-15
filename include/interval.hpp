// Copyright 2014 Matthew Harvey

#ifndef GUARD_interval_hpp_6955864893717628
#define GUARD_interval_hpp_6955864893717628

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
#include <jewel/exception.hpp>

namespace swx
{

JEWEL_DERIVED_EXCEPTION(IntervalException, jewel::Exception);
JEWEL_DERIVED_EXCEPTION(IntervalAlreadyClosedException, IntervalException);
JEWEL_DERIVED_EXCEPTION(InvalidIntervalException, IntervalException);

/**
 * Represent an interval of time. It may be open (still running) or
 * closed (has an end).
 */
class Interval
{
// special member functions
public:

	/**
	 * @param p_beginning Time at which the interval begins. Defaults
	 * to NOW.
	 */
	explicit Interval
	(	boost::posix_time::ptime const& p_beginning =
			boost::posix_time::second_clock::local_time()
	);

	Interval(Interval const& rhs) = default;
	Interval(Interval&& rhs) = default;
	Interval& operator=(Interval const& rhs) = default;
	Interval& operator=(Interval&& rhs) = default;
	~Interval() = default;

// ordinary member functions
public:
	
	/**
	 * Close the interval.
	 *
	 * @param p_end Time at which to close the interval.
	 * Defaults to NOW.
	 *
	 * @throws IntervalAlreadyClosedException if already closed.
	 *
	 * @throws InvalidIntervalException if p_end is earlier
	 * than beginning();
	 */
	void close
	(	boost::posix_time::ptime const& p_ending =
			boost::posix_time::second_clock::local_time()
	);

	/**
	 * @returns the starting time of the Interval.
	 */
	boost::posix_time::ptime beginning() const;

	/**
	 * @returns a possibly-null boost::optional which contains
	 * the ending time of the Interval, if it is closed, or else
	 * is invalid if it is open.
	 */
	boost::optional<boost::posix_time::ptime> maybe_ending() const;

// member variables
private:
	boost::posix_time::ptime m_beginning;
	boost::optional<boost::posix_time::ptime> m_maybe_ending;


};  // class Interval


// NON-MEMBER FUNCTIONS

/**
 * @returns \e true iff the Interval is open.
 */
bool is_open(Interval const& p_interval);

/**
 * @returns \e true iff the Interval is closed.
 */
bool is_closed(Interval const& p_interval);


}  // namespace swx

#endif  // GUARD_interval_hpp_6955864893717628
