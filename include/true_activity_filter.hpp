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

#ifndef GUARD_true_activity_filter_hpp_8735450957312925
#define GUARD_true_activity_filter_hpp_8735450957312925

#include "activity_filter.hpp"
#include <string>

namespace swx
{

/**
 * Represents an ActivityFilter that is trivially satisfied by
 * any activity.
 */
class TrueActivityFilter: public ActivityFilter
{
// special member functions
public:
    TrueActivityFilter();
    TrueActivityFilter(TrueActivityFilter const& rhs) = delete;
    TrueActivityFilter(TrueActivityFilter&& rhs) = delete;
    TrueActivityFilter& operator=(TrueActivityFilter const& rhs) = delete;
    TrueActivityFilter& operator=(TrueActivityFilter&& rhs) = delete;
    virtual ~TrueActivityFilter();

// inherited virtual functions
private:
    virtual bool does_match(std::string const& p_str) const;

};  // class TrueActivityFilter

}  // namespace swx

#endif  // GUARD_true_activity_filter_hpp_8735450957312925
