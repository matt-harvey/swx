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

#include "parsed_arguments.hpp"
#include <algorithm>
#include <cassert>
#include <iterator>
#include <set>
#include <vector>
#include <unordered_set>

using std::back_inserter;
using std::copy;
using std::string;
using std::vector;

namespace swx
{

ParsedArguments::ParsedArguments(vector<string> const& p_raw_args)
{
	for (auto it = p_raw_args.begin(); it != p_raw_args.end(); ++it)
	{
		auto const& arg = *it;
		if (arg == "--")
		{
			copy(it + 1, p_raw_args.end(), back_inserter(m_ordinary_args));
			break;
		}
		else if (!arg.empty() && (arg[0] == '-'))
		{
			m_single_character_flags.insert(arg.begin() + 1, arg.end());
		}
		else
		{
			m_ordinary_args.push_back(*it);	
		}
	}
}

vector<string>
ParsedArguments::ordinary_args() const
{
	return m_ordinary_args;
}

string
ParsedArguments::single_character_flags() const
{
	string const ret
	(	m_single_character_flags.begin(),
		m_single_character_flags.end()
	);
#	ifndef NDEBUG
		for (auto it = ret.begin(); it != ret.end(); ++it)
		{
			auto const next = it + 1;
			if (next != ret.end())
			{
				assert (*it < *next);
			}
		}
#	endif
	return ret;
}

}  // namespace swx
