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

#include "activity_filter.hpp"
#include "exact_activity_filter.hpp"
#include "ordinary_activity_filter.hpp"
#include "regex_activity_filter.hpp"
#include "true_activity_filter.hpp"
#include <cassert>
#include <string>

using std::string;

namespace swx
{

ActivityFilter*
ActivityFilter::create(string const& p_comparitor, Type p_type)
{
    switch (p_type)
    {
    case Type::ordinary:
        return new OrdinaryActivityFilter(p_comparitor);
    case Type::exact:
        return new ExactActivityFilter(p_comparitor);
    case Type::regex:
        return new RegexActivityFilter(p_comparitor);
    case Type::always_true:
        return new TrueActivityFilter;
    default:
        assert (false);  // we whould never reach here
    }
}

ActivityFilter::~ActivityFilter() = default;

bool
ActivityFilter::matches(string const& p_str) const
{
    return does_match(p_str);
}

string
ActivityFilter::replace(string const& p_old_str, string const& p_substitution) const
{
    return matches(p_old_str) ? do_replace(p_old_str, p_substitution) : p_old_str;
}

string
ActivityFilter::do_replace(string const& p_old_str, string const& p_substitution) const
{
    (void)p_old_str;  // silence compiler warning re. unused param.
    return p_substitution;
}

}  // namespace swx
