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

#include "command_category.hpp"
#include <cassert>
#include <string>

using std::string;

namespace swx
{

CommandCategory::CommandCategory
(   string const& p_label,
    SortOrder p_sort_order
):
    m_label(p_label),
    m_sort_order(p_sort_order)
{
}

bool
CommandCategory::operator<(CommandCategory const& rhs) const
{
    if (m_sort_order < rhs.m_sort_order)
    {
        return true;
    }
    if (m_sort_order > rhs.m_sort_order)
    {
        return false;
    }
    assert (m_sort_order == rhs.m_sort_order);
    return label() < rhs.label();
}

std::string
CommandCategory::label() const
{
    return m_label;
}

}  // namespace swx
