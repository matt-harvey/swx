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

#include "string_utilities.hpp"
#include <cassert>
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>

using std::getline;
using std::string;
using std::stringstream;
using std::vector;

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

}  // namespace swx
