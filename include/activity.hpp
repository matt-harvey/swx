/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_activity_hpp_8376698409382579
#define GUARD_activity_hpp_8376698409382579

#include "interval.hpp"
#include "swx_exception.hpp"
#include <jewel/exception.hpp>
#include <string>

namespace swx
{

JEWEL_DERIVED_EXCEPTION(ActivityException, SwxException);
JEWEL_DERIVED_EXCEPTION(UnclosedStintException, ActivityException);
JEWEL_DERIVED_EXCEPTION(StintOrderException, ActivityException);

/**
 * Represents a certain Activity, which may be worked on during
 * various "stints" of activity.
 */
class Activity
{
public:
	
	explicit Activity(std::string const& p_name);
	
	/**
	 * Add another stint of activity to the sequence of
	 * stints comprising work on this activity.
	 *
	 * @throws UnclosedStintException if the previous stint
	 * is not yet closed.
	 *
	 * @throws StintOrderException if the previous stint was
	 * closed at a time that is earlier than this stint
	 * begins.
	 */
	void push_stint(Interval const& p_stint);

private:
	
	bool is_valid() const;

	std::string m_name;
	std::vector<Interval> m_stints;

};  // class Activity

}  // namespace swx

#endif  // GUARD_activity_hpp_8376698409382579
