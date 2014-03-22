/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_activity_hpp_8376698409382579
#define GUARD_activity_hpp_8376698409382579

#include "interval.hpp"
#include "seconds.hpp"
#include <string>
#include <vector>

namespace swx
{

/**
 * Represents a certain Activity, which may be worked on during
 * various "stints" of activity.
 */
class Activity
{
// special member functions
public:
	explicit Activity(std::string const& p_name);
	
// ordinary member functions
public:

	std::string name() const;

	/**
	 * Add another stint of activity to the sequence of
	 * stints comprising work on this activity.
	 */
	void push_stint(Interval const& p_stint);

	std::vector<Interval> const& stints() const;

	Seconds duration() const;

// member variables
private:
	std::string m_name;
	std::vector<Interval> m_stints;

};  // class Activity

}  // namespace swx

#endif  // GUARD_activity_hpp_8376698409382579
