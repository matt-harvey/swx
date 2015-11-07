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
 * Abstract class representing a test against which a given string can be
 * evaluated, where the string represents the name of an activity. Also
 * supports replacing the matched part of the evaluated string with a new
 * string.
 *
 * TODO HIGH PRIORITY Write tests for this and its subclasses.
 */
class ActivityFilter
{
public:
// nested types
    enum class Type
    {
        ordinary,
        exact,
        regex,
        always_true
    };

// static factory function
public:
    /**
     * Caller receives ownership the pointer.
     */
    static ActivityFilter* create(std::string const& p_comparitor, Type p_type);

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
    bool matches(std::string const& p_str) const;

    /**
     * If \e p_old_str satisfies this filter, perform a replacement according
     * to the rule for this filter, replacing the matched text with \e p_substitution
     * and returning the result as a new string.
     */
    std::string replace
    (   std::string const& p_old_str,
        std::string const& p_substitution
    ) const;

// virtual member functions
private:
    virtual bool does_match(std::string const& p_str) const = 0;

    virtual std::string do_replace
    (   std::string const& p_old_str,
        std::string const& p_substitution
    ) const;

};  // class ActivityFilter

}  // namespace swx

#endif  // GUARD_activity_filter_hpp_5441199953407608
