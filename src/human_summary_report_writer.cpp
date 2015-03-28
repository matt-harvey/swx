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
#include <iterator>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::back_inserter;
using std::endl;
using std::fixed;
using std::left;
using std::make_pair;
using std::map;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::right;
using std::runtime_error;
using std::setprecision;
using std::setw;
using std::string;
using std::transform;
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
	map<ActivityNode, ActivityInfo> const& p_activity_info_map
)
{
	string::size_type left_col_width = 0;
	map<ActivityNode, ActivityInfo> activity_info_map = p_activity_info_map;

	unsigned int max_num_components = 1;

	// TODO Improve output if -b or -e options are passed.

	if (m_show_tree) 
	{
		// Calculate the greatest number of components
		// TODO There's probably some kind of "max" algorithm we could use here.
		for (auto const& pair: activity_info_map)
		{
			auto const num_components = pair.first.num_components();
			if (num_components > max_num_components)
			{
				max_num_components = num_components;
			}
		}

		// Make all the leaves have the same number of components
		vector<pair<ActivityNode, ActivityInfo>> temp;
		transform
		(	activity_info_map.begin(),
			activity_info_map.end(),
			back_inserter(temp),
			[max_num_components](pair<ActivityNode, ActivityInfo> const& p) {
				auto node = p.first;
				node.set_num_components(max_num_components);
				return make_pair(node, p.second);
			}
		);
		// TODO This sucks
		activity_info_map.clear();
		activity_info_map = map<ActivityNode, ActivityInfo>(temp.begin(), temp.end());

		// TODO MEDIUM PRIORITY tidy this. Split into separate functions.

		unsigned int max_level = max_num_components - 1;
		for (unsigned int examined_level = 0; examined_level != max_level; ++examined_level)
		{
			// Insert higher level activities into map
			for (auto const& pair: activity_info_map)
			{
				map<ActivityNode, ActivityInfo> parent_map;
				auto const& activity_node = pair.first;
				auto const& activity_info = pair.second;
				if (activity_node.level() == examined_level)
				{
					auto const parent_activity_node = activity_node.parent();
					auto const map_iter = activity_info_map.find(parent_activity_node);
					if (map_iter == activity_info_map.end())
					{
						// The parent activity is not already in the map, so insert it.
						auto parent_info = activity_info;
						parent_info.num_children = 1;
						parent_map.insert(make_pair(parent_activity_node, parent_info));
					}
					else
					{
						// The parent activity is in the map; just update it.
						auto& parent_info = map_iter->second;
						auto& parent_accum = parent_info.seconds;
						auto const activity_accum = activity_info.seconds;
						++parent_info.num_children;
						if (!addition_is_safe(parent_accum, activity_accum))
						{
							ostringstream oss;
							enable_exceptions(oss);
							oss << "Time spent on activity \"" << activity_node.activity()
								<< "\" is too great to be totalled correctly.";
							throw runtime_error(oss.str());
						}
						assert (addition_is_safe(parent_accum, activity_accum));
						parent_accum += activity_accum;
						if (activity_info.beginning < parent_info.beginning)
						{
							parent_info.beginning = activity_info.beginning;
						}
						if (activity_info.ending > parent_info.ending)
						{
							parent_info.ending = activity_info.ending;
						}
					}
				}
				activity_info_map.insert(parent_map.begin(), parent_map.end());	
			}
		}
	}
	unsigned int max_level = 0;
	for (auto const& pair: activity_info_map)
	{
		auto const& activity_node = pair.first;
		auto const activity_width =
		(	m_show_tree ?
			activity_node.marginal_name().length() :
			activity_node.activity().length()
		);
		if (activity_width > left_col_width) left_col_width = activity_width;
		if (activity_node.level() > max_level) max_level = activity_node.level();
	}
	unsigned long long total_seconds = 0;
	unsigned int last_level = 0;
	for (auto const& pair: activity_info_map)
	{
		auto const& activity_node = pair.first;
		auto const& info = pair.second;
		auto const seconds = info.seconds;
		auto const level = activity_node.level();
		if (level == 0)
		{
			if (!addition_is_safe(total_seconds, seconds))
			{
				throw runtime_error
				(	"Time spent on activities is too great to be totalled safely."
				);
			}
			assert (addition_is_safe(total_seconds, seconds));
			total_seconds += seconds;
		}
		auto const parent_iter = activity_info_map.find(activity_node.parent());
		if (parent_iter != activity_info_map.end())
		{
			auto const& parent_info = parent_iter->second;

			// TODO brittle
			// We don't want to show an "only child" if it has the same name as its parent,
			// ignoring the "filler name" (".").
			if ((parent_info.num_children == 1) && activity_node.marginal_name() == ".")
			{
				continue;
			}
		}
		if (level > last_level) p_os << endl;
		print_label_and_rounded_hours
		(	p_os,
			(m_show_tree ? activity_node.marginal_name() : activity_node.activity()),
			seconds,
			(m_include_beginning? &(info.beginning): nullptr),
			(m_include_ending? &(info.ending): nullptr),
			left_col_width,
			(m_show_tree ? max_num_components - level - 1 : 0)
		);
		last_level = level;
	}
	p_os << endl;
	print_label_and_rounded_hours
	(	p_os,
		"TOTAL",
		total_seconds,
		nullptr,
		nullptr,
		left_col_width
	);
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
	if (p_activity_depth == 0)
	{
		indent = "";
	}
	else
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
		p_os << "    " << time_point_to_stamp(*p_beginning, time_format(), formatted_buf_len());
	}
	if (p_ending != nullptr)
	{
		p_os << "    "<< time_point_to_stamp(*p_ending, time_format(), formatted_buf_len());
	}
	p_os << endl;
	return;
}

}  // namespace swx
