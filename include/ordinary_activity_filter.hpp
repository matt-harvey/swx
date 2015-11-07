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

#ifndef GUARD_ordinary_activity_filter_hpp_8516776239740093
#define GUARD_ordinary_activity_filter_hpp_8516776239740093

#include "activity_filter.hpp"
#include <string>

namespace swx
{

/**
 * Represents an ActivityFilter that is satisfied by all and only those
 * activities where either their name is identical to the comparison string,
 * or they are a descendent activity of an activity whose name is identical
 * to the comparison string.
 */
class OrdinaryActivityFilter: public ActivityFilter
{
// special member functions
public:
    explicit OrdinaryActivityFilter(std::string const& p_comparitor);
    OrdinaryActivityFilter(OrdinaryActivityFilter const& rhs) = delete;
    OrdinaryActivityFilter(OrdinaryActivityFilter&& rhs) = delete;
    OrdinaryActivityFilter& operator=(OrdinaryActivityFilter const& rhs) = delete;
    OrdinaryActivityFilter& operator=(OrdinaryActivityFilter&& rhs) = delete;
    virtual ~OrdinaryActivityFilter();

// inherited virtual member functions
private:
    virtual bool do_test(std::string const& p_str) const override;

// data members
private:
    std::string const m_comparitor;

};  // class OrdinaryActivityFilter

}  // namespace swx

#endif  // GUARD_ordinary_activity_filter_hpp_8516776239740093
