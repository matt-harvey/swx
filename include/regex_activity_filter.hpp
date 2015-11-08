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

#ifndef GUARD_regex_activity_filter_hpp_1239507264103511
#define GUARD_regex_activity_filter_hpp_1239507264103511

#include "activity_filter.hpp"
#include <regex>
#include <string>

namespace swx
{

/**
 * Represents an ActivityFilter that is satisfied by all and only those
 * activities the name of which matches the comparison string considered as
 * a POSIX extended regular expression.
 */
class RegexActivityFilter: public ActivityFilter
{
public:
    explicit RegexActivityFilter(std::string const& p_comparitor);
    RegexActivityFilter(RegexActivityFilter const& rhs) = delete;
    RegexActivityFilter(RegexActivityFilter&& rhs) = delete;
    RegexActivityFilter& operator=(RegexActivityFilter const& rhs) = delete;
    RegexActivityFilter& operator=(RegexActivityFilter&& rhs) = delete;
    virtual ~RegexActivityFilter();

// inherited virtual member functions
private:
    virtual bool does_match(std::string const& p_str) const override;

    virtual std::string do_replace
    (   std::string const& p_old_str,
        std::string const& p_substitution
    ) const override;

// data members
private:
    std::regex const m_comparitor;

};  // class RegexActivityFilter

}  // namespace swx

#endif  // GUARD_regex_activity_filter_hpp_1239507264103511
