/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_squish_hpp_40349379060952634
#define GUARD_squish_hpp_40349379060952634

#include <string>

namespace swx
{

template <typename StringIter>
std::string squish(StringIter beg, StringIter const& end);

template <typename StringIter>
std::string
squish(StringIter beg, StringIter const& end)
{
	std::string ret;
	if (beg != end)
	{
		ret += *beg;
		++beg;
		for ( ; beg != end; ++beg)
		{
			ret += (" " + *beg);
		}
	}
	return ret;
}

}  // namespace swx

#endif  // GUARD_squish_hpp_40349379060952634
