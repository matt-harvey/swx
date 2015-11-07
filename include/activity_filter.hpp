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

#ifndef GUARD_activity_filter_hpp_5441199953407608
#define GUARD_activity_filter_hpp_5441199953407608

#include <string>

namespace swx
{

/**
 * Abstract functor class representing a test against which a given string can be
 * evaluated, where the string represents the name of an activity.
 */
class ActivityFilter
{
// special member functions
public:
    ActivityFilter() = default;
    ActivityFilter(ActivityFilter const& rhs) = delete;
    ActivityFilter(ActivityFilter&& rhs) = delete;
    ActivityFilter& operator=(ActivityFilter const& rhs) = delete;
    ActivityFilter& operator=(ActivityFilter&& rhs) = delete;
    virtual ~ActivityFilter();

// ordinary member functions
public:
    /**
     * @param p_str (the name of) an activity.
     * @return \e true if an only if \e p_str satisfies the test.
     */
    bool operator()(std::string const& p_str) const;

// virtual member functions
private:
    virtual bool do_test(std::string const& p_str) const = 0;

};  // class ActivityFilter

}  // namespace swx

#endif  // GUARD_activity_filter_hpp_5441199953407608
