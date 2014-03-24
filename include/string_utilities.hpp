/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_string_utilities_hpp_7073320819564779
#define GUARD_string_utilities_hpp_7073320819564779

#include <sstream>
#include <string>

namespace swx
{

std::string trim(std::string const& p_string);

template <typename StringIter>
std::string squish(StringIter beg, StringIter const& end);

// TEMPLATE IMPLEMENTATIONS

template <typename StringIter>
std::string
squish(StringIter beg, StringIter const& end)
{
	std::ostringstream oss;
	if (beg != end)
	{
		oss << trim(*beg);
		for (++beg; beg != end; ++beg)
		{
			oss << ' ' << trim(*beg);
		}
	}
	return oss.str();
}

}  // namespace swx

#endif  // GUARD_string_utilities_hpp_7073320819564779
