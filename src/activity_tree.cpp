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

#include "activity_tree.hpp"
#include "activity_stats.hpp"
#include "activity_node.hpp"
#include "arithmetic.hpp"
#include "stream_flag_guard.hpp"
#include "string_utilities.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using std::endl;
using std::function;
using std::make_pair;
using std::map;
using std::max;
using std::move;
using std::ostream;
using std::pair;
using std::set;
using std::string;
using std::vector;

namespace swx
{

ActivityTree::ActivityTree(map<string, ActivityStats> const& p_stats):
    m_root(ActivityNode(""))
{
    // Calculate the greatest number of components of any activity
    vector<string>::size_type depth = 0;
    for (auto const& p: p_stats) depth = max(depth, split(p.first).size());

    // Make all the leaf activities have the same number of components, and insert
    // them into the inheritance map.
    for (auto const& pair: p_stats)
    {
        m_map.insert(make_pair(ActivityNode(pair.first, depth), ActivityData(pair.second)));
    }

    // Go through each generation, starting with the leaves, and building the parent
    // generation of each. But first, cover the case where there are no nodes. Even
    // then, we want a root node.
    if (m_map.empty()) m_map.insert(make_pair(m_root, ActivityData()));
    auto current_generation = m_map;
    assert (!current_generation.empty());
    while (current_generation.begin()->first != m_root)
    {
        decltype(current_generation) parent_generation;
        for (auto const& pair: current_generation)
        {
            auto const& node = pair.first;
            auto const& stats = pair.second.stats;
            auto const parent_node = node.parent();
            auto const parent_iter = parent_generation.find(parent_node);
            if (parent_iter == parent_generation.end())
            {
                ActivityData const data(stats, {node});
                parent_generation.insert(make_pair(parent_node, data));
            }
            else
            {
                parent_iter->second.children.insert(node);
                parent_iter->second.stats += stats;
            }
        }
        m_map.insert(parent_generation.begin(), parent_generation.end());
        current_generation = move(parent_generation);
        assert (!current_generation.empty());
    }
}

void
ActivityTree::print
(   ostream& p_os,
    ActivityNode const& p_node,
    string const& p_label,
    unsigned int p_depth,
    PrintNode const& p_print_node
) const
{
    assert (m_map.find(p_node) != m_map.end());
    auto const& data = m_map.find(p_node)->second;
    string label_carried_forward;
    if (data.children.size() == 1)
    {
        label_carried_forward = p_label + ' ';
    }
    else
    {
        p_print_node(p_os, p_depth, p_label, data.stats);
        ++p_depth;
    }
    for (auto const& child: data.children)
    {
        auto const label = trim(label_carried_forward + child.marginal_name());
        print(p_os, child, label, p_depth, p_print_node);
    }
    return;
}

void
ActivityTree::print(ostream& p_os, PrintNode const& p_print_node) const
{
    print(p_os, m_root, "", 0, p_print_node);
    return;
}

ActivityTree::ActivityData::ActivityData
(   ActivityStats const& p_stats,
    set<ActivityNode> const& p_children
):
    stats(p_stats),
    children(p_children)
{
}

}  // namespace swx
