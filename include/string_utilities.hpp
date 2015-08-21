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

#include "stream_utilities.hpp"
#include <sstream>
#include <string>
#include <vector>

namespace swx
{

/**
 * @returns a string that is identical to \e p_string except in that
 * whitespace has been trimmed from the beginning and end.
 */
std::string trim(std::string const& p_string);

/**
 * @param p_beg iterator pointing to the first of a sequence of strings
 *
 * @param p_end iterator to one-past-the-end of the sequence of strings
 *
 * @returns a string produced by trimming the whitespace from the beginning and
 * end of each string, and then joining them together with a single space between
 * each.
 */
template <typename StringIter>
std::string squish(StringIter p_beg, StringIter const& p_end);

/**
 * @param p_str string to split
 *
 * @param p_delimiter
 *
 * @returns a vector of strings derived from splitting \e p_str at each occurrence
 * of \e p_delimiter. The returned strings omit the delimiter.
 */
std::vector<std::string> split(std::string const& p_str, char p_delimiter = ' ');

// TEMPLATE IMPLEMENTATIONS

template <typename StringIter>
std::string
squish(StringIter p_beg, StringIter const& p_end)
{
    std::ostringstream oss;
    enable_exceptions(oss);
    if (p_beg != p_end)
    {
        oss << trim(*p_beg);
        for (++p_beg; p_beg != p_end; ++p_beg)
        {
            oss << ' ' << trim(*p_beg);
        }
    }
    return trim(oss.str());
}

}  // namespace swx

#endif  // GUARD_string_utilities_hpp_7073320819564779
