/*
 * Copyright 2014 Matthew Harvey
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "placeholder.hpp"
#include "stream_utilities.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using std::back_inserter;
using std::copy;
using std::ostringstream;
using std::size_t;
using std::string;
using std::vector;

namespace swx
{

namespace
{

	char tree_traversal_char()
	{
		return '_';
	}

	/**
	 * @returns \e true if p_str successfully expands into an
	 * activity string, in the context of p_time_log; otherwise, returns \e
	 * false.
	 *
	 * If successful, pushes the resulting activity components to \e p_vec;
	 * otherwise, it leaves \e p_vec unchanged.
	 */
	bool parse_placeholder
	(	string const& p_str,
		vector<string>& p_vec,
		TimeLog& p_time_log
	)
	{
		size_t depth = 0;
		if (p_str.empty())
		{
			return false;
		}
		for (char c: p_str)
		{
			if (c == tree_traversal_char()) ++depth;
			else return false;
		}
		if (p_time_log.is_active())
		{
			auto const last_activities = p_time_log.last_activities(1);
			assert (!last_activities.empty());
			string const activity = last_activities.front();
			auto const components = split(activity, ' ');
			assert (depth > 0);
			--depth;
			auto const b = components.begin();
			auto it = components.end();
			for ( ; (it != b) && (depth != 0); --it, --depth)
			{
			}
			copy(b, it, back_inserter(p_vec));
		}
		else
		{
			p_vec.push_back("");
		}
		return true;
	}

}  // end anonymous namespace

vector<string>
expand_placeholders(vector<string> const& p_components, TimeLog& p_time_log)
{
	vector<string> vec;
	vec.reserve(p_components.size());
	for (auto const& component: p_components)
	{
		if (!parse_placeholder(component, vec, p_time_log))
		{
			if (!component.empty()) vec.push_back(component);
		}
	}
	return vec;
}

vector<string>
placeholder_help(string::size_type p_left_column_width)
{
	vector<string> ret;
	vector<string>::size_type const num_lines = 3;
	string::size_type const min_width = num_lines + 1;
	if (min_width > p_left_column_width) p_left_column_width = min_width;
	for (string::size_type i = 1; i <= num_lines; ++i)
	{
		ostringstream oss;
		enable_exceptions(oss);
		oss << string(i, tree_traversal_char())
		    << string(p_left_column_width - i, ' ');
		switch (i)
		{
		case 1:
			oss << "Expands into name of current activity "
			    << "(or empty string if inactive)";
			break;
		case 2:
			oss << "Expands into name of parent of current activity "
			    << "(or empty string if no parent)";
			break;
		case 3:
			oss << "Expands into name of parent of parent (etc.)";
			break;
		default:
			assert (false);
		}
		ret.push_back(oss.str());
	}
	return ret;	
}

}  // namespace swx
