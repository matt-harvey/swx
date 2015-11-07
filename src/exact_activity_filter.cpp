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

#include "exact_activity_filter.hpp"
#include <cassert>
#include <string>

using std::string;

namespace swx
{

ExactActivityFilter::ExactActivityFilter(string const& p_comparitor):
    m_comparitor(p_comparitor)
{
}

ExactActivityFilter::~ExactActivityFilter() = default;

bool
ExactActivityFilter::do_test(string const& p_str) const
{
    return p_str == m_comparitor;
}

}  // namespace swx
