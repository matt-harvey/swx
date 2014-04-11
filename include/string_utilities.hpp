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
