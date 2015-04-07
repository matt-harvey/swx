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

ActivityTree::ActivityTree(map<string, ActivityStats> const& p_stats_map):
	m_root(ActivityNode(""))
{
	// Calculate the greatest number of components of any activity
	auto const depth = accumulate
	(	p_stats_map.begin(),
		p_stats_map.end(),
		0,
		[](size_t n, pair<string, ActivityStats> const& p)
		{
			return max(n, split(p.first).size());
		}
	);

	// Make all the leaf activities have the same number of components, and insert
	// them into the inheritance map.
	for (auto const& pair: p_stats_map)
	{
		m_map.insert(make_pair(ActivityNode(pair.first, depth), ActivityData(pair.second)));
	}

	// Go through each generation, starting with the leaves, and building the parent
	// generation of each. But first, cover the case where there are no nodes. Even
	// then, we want a root node.
	if (m_map.empty())
	{
		m_map.insert(make_pair(m_root, ActivityData()));
		return;
	}
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
	assert (m_map.find(p_node) != m_map.end());
	auto const& data = m_map.find(p_node)->second;
	if (p_concatenate_to_previous)
	{
		p_os << p_node.marginal_name() << ' ';
	}
	else
	{
		StreamFlagGuard guard(p_os);
		if (m_root != p_node) p_os << endl;
		auto const marginal_name = p_node.marginal_name();
		auto const name_str = (marginal_name.empty() ? "" : marginal_name + ' ');
		auto const seconds = data.stats.seconds;
		p_os << string(p_depth * (p_width + 3), ' ') << '['
		     << fixed << setprecision(p_precision) << right << setw(p_width)
			 << round(seconds / 60.0 / 60.0, p_rounding_numerator, p_rounding_denominator)
			 << " ] " << left << name_str;
	}
	auto const has_single_child = (data.children.size() == 1);
	for (auto const& child: data.children)
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

ActivityTree::ActivityData::ActivityData
(	ActivityStats const& p_stats,
	set<ActivityNode> const& p_children
):
	stats(p_stats),
	children(p_children)
{
}

}  // namespace swx
