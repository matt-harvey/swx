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

#include "activity_node.hpp"
#include "string_utilities.hpp"
#include <algorithm>
#include <cassert>
#include <map>
#include <numeric>
#include <string>

using std::accumulate;
using std::map;
using std::max;
using std::min;
using std::string;
using std::vector;

namespace swx
{

ActivityNode::ActivityNode(string const& p_activity):
    m_components(split(p_activity))
{
}

ActivityNode::ActivityNode(string const& p_activity, unsigned int p_depth):
    m_components(split(p_activity))
{
    m_components.resize(p_depth);
}

ActivityNode::ActivityNode
(   vector<string>::const_iterator const& p_begin,
    vector<string>::const_iterator const& p_end
):
    m_components(p_begin, p_end)
{
}

bool
ActivityNode::operator<(ActivityNode const& rhs) const
{
    return m_components < rhs.m_components;
}

bool
ActivityNode::operator==(ActivityNode const& rhs) const
{
    return m_components == rhs.m_components;
}

bool
ActivityNode::operator!=(ActivityNode const& rhs) const
{
    return !(*this == rhs);
}

string
ActivityNode::activity() const
{
    return squish(m_components.begin(), m_components.end());
}

string
ActivityNode::marginal_name() const
{
    return m_components.empty() ? "" : m_components.back();
}

ActivityNode
ActivityNode::parent() const
{
    assert (!m_components.empty());
    return ActivityNode(m_components.begin(), m_components.end() - 1);
}

}  // namespace swx
