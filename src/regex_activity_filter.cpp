/*
 * Copyright 2015 Matthew Harvey
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

#include "regex_activity_filter.hpp"
#include <regex>
#include <string>

using std::regex;
using std::regex_replace;
using std::regex_search;
using std::string;

namespace swx
{

RegexActivityFilter::RegexActivityFilter(string const& p_string):
    m_comparitor(p_string, regex::extended | regex::optimize)
{
}

RegexActivityFilter::~RegexActivityFilter() = default;

bool
RegexActivityFilter::does_match(string const& p_str) const
{
    return regex_search(p_str, m_comparitor);
}

string
RegexActivityFilter::do_replace
(   string const& p_old_str,
    string const& p_substitution
) const
{
    return regex_replace(p_old_str, m_comparitor, p_substitution);
}

}  // namespace swx
