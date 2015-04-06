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
#include "activity_info.hpp"
#include "activity_node.hpp"
#include "arithmetic.hpp"
#include "stream_flag_guard.hpp"
#include "string_utilities.hpp"
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <ostream>
#include <set>
#include <string>
#include <utility>

using std::accumulate;
using std::endl;
using std::fixed;
using std::left;
using std::make_pair;
using std::map;
using std::max;
using std::move;
using std::ostream;
using std::pair;
using std::right;
using std::set;
using std::setw;
using std::setprecision;
using std::string;

namespace swx
{

ActivityTree::ActivityTree(map<string, ActivityInfo> const& p_info_map):
	m_root(ActivityNode(""))
{
	// Calculate the greatest number of components of any activity
	auto const depth = accumulate
	(	p_info_map.begin(),
		p_info_map.end(),
		0,
		[](size_t n, pair<string, ActivityInfo> const& p)
		{
			return max(n, split(p.first).size());
		}
	);

	// Make all the leaf activities have the same number of components, and insert
	// them into the inheritance map.
	for (auto const& pair: p_info_map)
	{
		auto node = ActivityNode(pair.first, depth);
		auto const info = pair.second;
		m_inheritance_map.insert(make_pair(node, set<ActivityNode>{}));
		m_info_map.insert(make_pair(node, info));
	}

	// Go through each generation, starting with the leaves, and building the parent
	// generation of each. But first, cover the case where there are no nodes. Even
	// then, we want a root node.
	if (m_inheritance_map.empty())
	{
		m_inheritance_map.insert(make_pair(m_root, set<ActivityNode>{}));
		m_info_map.insert(make_pair(m_root, ActivityInfo()));
	}
	auto current_generation = m_inheritance_map;
	while (current_generation.size() != 1)  // until root node reached
	{
		decltype(current_generation) parent_generation;
		for (auto const& pair: current_generation)
		{
			auto const& node = pair.first;
			auto const parent_node = node.parent();
			auto const parent_iter = parent_generation.find(parent_node);
			if (parent_iter == parent_generation.end())
			{
				parent_generation.insert(make_pair(parent_node, set<ActivityNode>{node}));
				m_info_map.insert(make_pair(parent_node, info(node)));
			}
			else
			{
				auto& children = parent_iter->second;
				children.insert(node);
				auto const info_iter = m_info_map.find(parent_node);
				assert (info_iter != m_info_map.end());
				auto& parent_info = info_iter->second;
				parent_info += info(node);
			}
		}
		m_inheritance_map.insert(parent_generation.begin(), parent_generation.end());
		current_generation = move(parent_generation);
	}
}

set<ActivityNode> const&
ActivityTree::children(ActivityNode const& p_node) const
{
	auto const iter = m_inheritance_map.find(p_node);
	assert (iter != m_inheritance_map.end());
	return iter->second;
}

ActivityInfo const&
ActivityTree::info(ActivityNode const& p_node) const
{
	auto const iter = m_info_map.find(p_node);
	assert (iter != m_info_map.end());
	return iter->second;
}

void
ActivityTree::print
(	ostream& p_os,
	ActivityNode const& p_node,
	unsigned int p_depth,
	bool p_concatenate_to_previous,
	unsigned int p_precision,
	unsigned int p_width,
	unsigned int p_rounding_numerator,
	unsigned int p_rounding_denominator
) const
{
	if (p_concatenate_to_previous)
	{
		p_os << p_node.marginal_name() << ' ';
	}
	else
	{
		StreamFlagGuard guard(p_os);
		auto const seconds = info(p_node).seconds;
		if (m_root != p_node) p_os << endl;
		p_os << string(p_depth * (p_width + 3), ' ') << '['
		     << fixed << setprecision(p_precision) << right << setw(p_width)
			 << round(seconds / 60.0 / 60.0, p_rounding_numerator, p_rounding_denominator)
			 << " ] " << left << p_node.marginal_name() << ' ';
	}
	auto const& child_nodes = children(p_node);
	auto const has_single_child = (child_nodes.size() == 1);
	for (auto const& child: child_nodes)
	{
		print
		(	p_os,
			child,
			p_depth + (p_concatenate_to_previous ? 0 : 1),
			has_single_child,
			p_precision,
			p_width,
			p_rounding_numerator,
			p_rounding_denominator
		);
	}
	return;
}

void
ActivityTree::print
(	ostream& p_os,
	unsigned int p_precision,
	unsigned int p_width,
	unsigned int p_rounding_numerator,
	unsigned int p_rounding_denominator
) const
{
	print
	(	p_os,
		m_root,
		0,
		false,
		p_precision,
		p_width,
		p_rounding_numerator,
		p_rounding_denominator
	);
	p_os << endl;
	return;
}

}  // namespace swx
