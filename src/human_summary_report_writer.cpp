/*
 * Copyright 2014 Matthew Harvey
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

#include "human_summary_report_writer.hpp"
#include "activity_info.hpp"
#include "activity_node.hpp"
#include "arithmetic.hpp"
#include "stint.hpp"
#include "stream_flag_guard.hpp"
#include "stream_utilities.hpp"
#include "string_utilities.hpp"
#include "summary_report_writer.hpp"
#include "time_point.hpp"
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::endl;
using std::fixed;
using std::left;
using std::make_pair;
using std::map;
using std::min;
using std::max;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::right;
using std::runtime_error;
using std::setprecision;
using std::setw;
using std::string;
using std::vector;

namespace swx
{

HumanSummaryReportWriter::HumanSummaryReportWriter
(	vector<Stint> const& p_stints,
	Options const& p_options,
	bool p_include_beginning,
	bool p_include_ending,
	bool p_show_tree
):
	SummaryReportWriter(p_stints, p_options),
	m_include_beginning(p_include_beginning),
	m_include_ending(p_include_ending),
	m_show_tree(p_show_tree)
{
}

HumanSummaryReportWriter::~HumanSummaryReportWriter() = default;

void
HumanSummaryReportWriter::do_write_summary
(	ostream& p_os,
	map<std::string, ActivityInfo> const& p_activity_info_map
)
{
	if (m_show_tree) write_tree_summary(p_os, p_activity_info_map);
	else write_flat_summary(p_os, p_activity_info_map);
	return;
}

void
HumanSummaryReportWriter::print_label_and_rounded_hours
(	ostream& p_os,
	string const& p_label,
	unsigned long long p_seconds,
	TimePoint const* p_beginning,
	TimePoint const* p_ending,
	unsigned int p_left_col_width,
	unsigned int p_activity_depth
) const
{
	StreamFlagGuard guard(p_os);

	string indent;
	if (p_activity_depth != 0)
	{
		indent = string((p_activity_depth - 1) * 8, ' ') + "  |___ ";
	}

	p_os << indent << left << setw(p_left_col_width) << p_label << ' ';
	p_os << fixed
	     << setprecision(output_precision())
		 << right
		 << setw(output_width())
		 << seconds_to_rounded_hours(p_seconds);
	guard.reset();

	if (p_beginning != nullptr)
	{
		p_os << "    "
		     << time_point_to_stamp(*p_beginning, time_format(), formatted_buf_len());
	}
	if (p_ending != nullptr)
	{
		p_os << "    "
		     << time_point_to_stamp(*p_ending, time_format(), formatted_buf_len());
	}
	p_os << endl;

	return;
}

void
HumanSummaryReportWriter::write_flat_summary
(	ostream& p_os,
	map<std::string, ActivityInfo> const& p_activity_info_map
)
{
	string::size_type left_col_width = 0;
	map<ActivityNode, ActivityInfo> node_map;
	node_map.insert(p_activity_info_map.begin(), p_activity_info_map.end());
	for (auto const& pair: node_map)
	{
		auto const& node = pair.first;
		auto const activity_width = node.activity().length();
		if (activity_width > left_col_width) left_col_width = activity_width;
	}
	ActivityInfo total_info;
	for (auto const& pair: node_map)
	{
		auto const& node = pair.first;
		auto const& info = pair.second;
		total_info += info;
		print_label_and_rounded_hours
		(	p_os,
			node.activity(),
			info.seconds,
			(m_include_beginning? &(info.beginning): nullptr),
			(m_include_ending? &(info.ending): nullptr),
			left_col_width
		);
	}
	p_os << endl;
	print_label_and_rounded_hours
	(	p_os,
		"TOTAL",
		total_info.seconds,
		(m_include_beginning? &(total_info.beginning): nullptr),
		(m_include_ending? &(total_info.ending): nullptr),
		left_col_width
	);
	return;
}

void
HumanSummaryReportWriter::write_tree_summary
(	ostream& p_os,
	map<std::string, ActivityInfo> const& p_activity_info_map
)
{
	// TODO MEDIUM PRIORITY Improve output if -b or -e options are passed together with -t.
	
	// TODO ActivityNode should be responsible for constructing the tree given
	// a sequence of std::string (or of leaf level ActivityNode instances).
	// Each ActivityNode should own the set of its own children. Then the
	// ordering of ActivityNode with '<' shouldn't have to be as complex as we
	// won't have to put all the different generations in the same map.

	string::size_type left_col_width = 0;
	map<ActivityNode, ActivityInfo> node_map;

	unsigned int max_num_components = 1;

	// Calculate the greatest number of components of any activity
	for (auto const& pair: p_activity_info_map)
	{
		auto const& activity = pair.first;
		unsigned int const num_components = split(activity, ' ').size();
		max_num_components = max(num_components, max_num_components);
	}

	// Make all the leaf activities have the same number of components
	for (auto const& pair: p_activity_info_map)
	{
		auto const& activity = pair.first;
		auto const& info = pair.second;
		auto node = ActivityNode(activity);
		node.set_num_components(max_num_components);
		node_map.insert(make_pair(node, info));
	}

	// Go through each generation, starting with the leaves, and building
	// the parent generation of each.
	auto current_generation = node_map;
	for (decltype(max_num_components) i = 1; i != max_num_components; ++i)
	{
		decltype(current_generation) parent_generation;

		// Insert higher level activities into map
		for (auto const& pair: current_generation)
		{
			auto const& node = pair.first;
			auto const& info = pair.second;
			auto const parent_node = node.parent();
			auto const iter = parent_generation.find(parent_node);
			if (iter == parent_generation.end())
			{
				// The parent activity is not already in the map, so insert it.
				auto parent_info = info;
				parent_info.num_children = 1;
				parent_generation.insert(make_pair(parent_node, parent_info));
			}
			else
			{
				// The parent activity is in the map; just update it.
				auto& parent_info = iter->second;
				++parent_info.num_children;
				parent_info += info;
			}
		}
		node_map.insert(parent_generation.begin(), parent_generation.end());
		current_generation = parent_generation;
	}
	for (auto const& pair: node_map)
	{
		auto const width = pair.first.marginal_name().length();
		if (width > left_col_width) left_col_width = width;
	}
	ActivityInfo total_info;
	unsigned int last_num_components = max_num_components;
	for (auto const& pair: node_map)
	{
		auto const& node = pair.first;
		auto const& info = pair.second;
		total_info += info;
		auto const parent_iter = node_map.find(node.parent());
		if (parent_iter != node_map.end())
		{
			auto const& parent_info = parent_iter->second;
			if ((parent_info.num_children == 1) && node.marginal_name().empty())
			{
				continue;
			}
		}
		if (node.num_components() < last_num_components)
		{
			p_os << endl;
		}
		print_label_and_rounded_hours
		(	p_os,
			node.marginal_name(),
			info.seconds,
			(m_include_beginning? &(info.beginning): nullptr),
			(m_include_ending? &(info.ending): nullptr),
			left_col_width,
			node.num_components() - 1
		);
		last_num_components = node.num_components();
	}
	p_os << endl;
	print_label_and_rounded_hours
	(	p_os,
		"TOTAL",
		total_info.seconds,
		(m_include_beginning? &(total_info.beginning): nullptr),
		(m_include_ending? &(total_info.ending): nullptr),
		left_col_width
	);
	return;
}

}  // namespace swx
