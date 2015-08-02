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

#ifndef GUARD_command_category_hpp_8583306632276302
#define GUARD_command_category_hpp_8583306632276302

#include <string>

namespace swx
{

/**
 * Represents a category into which a command may fall for
 * the purpose of sorting and grouping commands when displayed in help
 * output.
 */
class CommandCategory
{
// nested types
public:
    using SortOrder = int;

// special member functions
public:

    /**
     * \param p_label A label for the category for display to the user.
     *
     * \param p_sort_order The higher the sort order, the later the category will be
     * sorted. Categories of equal sort order are sorted by label.
     */
    explicit CommandCategory(std::string const& p_label, SortOrder p_sort_order = 0);

    CommandCategory(CommandCategory const& rhs) = default;
    CommandCategory(CommandCategory&& rhs) = default;
    CommandCategory& operator=(CommandCategory const& rhs) = default;
    CommandCategory& operator=(CommandCategory&& rhs) = default;
    ~CommandCategory() = default;

// operators
public:
    bool operator<(CommandCategory const& rhs) const;

// ordinary member functions
public:
    std::string label() const;

// member variables
private:
    std::string m_label;
    SortOrder m_sort_order;

};  // class CommandCategory

}  // namespace swx

#endif  // GUARD_command_category_hpp_8583306632276302
