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

#include "ordinary_activity_filter.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>  // DEBUG
#include <string>

using std::equal;
using std::string;

namespace swx
{

OrdinaryActivityFilter::OrdinaryActivityFilter(string const& p_comparitor):
    m_comparitor(p_comparitor)
{
}

OrdinaryActivityFilter::~OrdinaryActivityFilter() = default;

bool
OrdinaryActivityFilter::do_test(string const& p_str) const
{
    auto const str_size = p_str.size();
    auto const comparitor_size = m_comparitor.size();
    return
        (str_size >= comparitor_size) &&
        equal(m_comparitor.begin(), m_comparitor.end(), p_str.begin()) &&
        (str_size == comparitor_size || *(p_str.begin() + comparitor_size) == ' ');
}

}  // namespace swx
