/*
 * Copyright 2014, 2015 Matthew Harvey
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

#include "string_utilities.hpp"
#include "stream_utilities.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using std::copy;
using std::getline;
using std::ostream_iterator;
using std::ostringstream;
using std::regex;
using std::regex_replace;
using std::string;
using std::stringstream;
using std::vector;

namespace swx
{

string
trim(string const& p_string)
{
    return regex_replace(p_string, regex("^\\s+|\\s+$"), string());
}

string
squash(string const& p_string)
{
    return trim(regex_replace(p_string, regex("\\s+"), " "));
}

vector<string>
split(string const& p_str, char p_delimiter)
{
    vector<string> ret;
    string::const_iterator it0, it1;
    for (it0 = it1 = p_str.begin(); it1 != p_str.end(); ++it1)
    {
        if (*it1 == p_delimiter)
        {
            ret.emplace_back(it0, it1);
            it0 = it1 + 1;
        }
    }
    if (!p_str.empty()) ret.emplace_back(it0, it1);
    return ret;
}

string
wrap
(   string const& p_string,
    string::size_type p_margin,
    string::size_type p_width
)
{
    // TODO LOW PRIORITY This could probably be tidied up a bit.
    auto const separator = string("\n") + string(p_margin, ' ');
    ostringstream oss;
    enable_exceptions(oss);

    string::const_iterator it0, it1, it2;
    for (it0 = it1 = it2 = p_string.begin(); it2 != p_string.end(); ++it2)
    {
        if (*it2 == ' ')
        {
            if (it2 - it0 + p_margin > p_width)
            {
                if (it0 != p_string.begin()) oss << separator;
                copy(it0, it1, ostream_iterator<char>(oss));
                it0 = it1 + 1;
            }
            it1 = it2;
        }
    }
    if (it0 != p_string.begin()) oss << separator;
    copy(it0, it1, ostream_iterator<char>(oss));
    if ((it2 - it0 + p_margin > p_width) && (it1 < it2))
    {
        oss << separator;
        ++it1;
    }
    if (it1 < it2)
    {
        copy(it1, it2, ostream_iterator<char>(oss));
    }

    return oss.str();
}

}  // namespace swx
