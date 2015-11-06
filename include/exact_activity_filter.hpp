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

#ifndef GUARD_exact_activity_filter_hpp_8016280060641598
#define GUARD_exact_activity_filter_hpp_8016280060641598

#include "activity_filter.hpp"
#include <string>

namespace swx
{

/**
 * Represents an ActivityFilter that is satisfied by all and only those activities
 * where their name is exactly equal to the comparison string.
 */
class ExactActivityFilter: public ActivityFilter
{
// special member functions
public:
    explicit ExactActivityFilter(std::string const& p_comparitor);
    ExactActivityFilter(ExactActivityFilter const& rhs) = delete;
    ExactActivityFilter(ExactActivityFilter&& rhs) = delete;
    ExactActivityFilter& operator=(ExactActivityFilter const& rhs) = delete;
    ExactActivityFilter& operator=(ExactActivityFilter&& rhs) = delete;
    virtual ~ExactActivityFilter();

// inherited virtual member functions
private:
    virtual bool does_match(std::string const& p_str) const override;

// data members
private:
    std::string const m_comparitor;

};  // class ExactActivityFilter

}  // namespace swx

#endif  // GUARD_exact_activity_filter_hpp_8016280060641598
