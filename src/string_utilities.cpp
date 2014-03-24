/*
 * Copyright 2014 Matthew Harvey
 */

#include "string_utilities.hpp"
#include <cassert>
#include <string>

using std::string;

namespace swx
{

string
trim(string const& p_string)
{
	auto it = p_string.begin();
	for ( ; it != p_string.end() && isspace(*it); ++it)
	{
	}
	string ret(it, p_string.end());
	it = ret.end();
	if (!ret.empty()) --it;
	string::size_type num_to_pop = 0;
	for ( ; it >= p_string.begin() && isspace(*it); --it, ++num_to_pop)
	{
	}
	assert (num_to_pop <= ret.size());
	ret.resize(ret.size() - num_to_pop);
	return ret;
}


}  // namespace swx
