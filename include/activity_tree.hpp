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

#ifndef GUARD_activity_tree_hpp_4902535711835388
#define GUARD_activity_tree_hpp_4902535711835388

#include "activity_node.hpp"
#include <map>
#include <set>
#include <ostream>
#include <string>

namespace swx
{

// begin forward declarations

struct ActivityInfo;

// end forward declarations

class ActivityTree
{
// special member functions
public:
	explicit ActivityTree(std::map<std::string, ActivityInfo> const& p_info_map);
	ActivityTree(ActivityTree const& rhs) = delete;
	ActivityTree(ActivityTree&& rhs) = delete;
	ActivityTree& operator=(ActivityTree const& rhs) = delete;
	ActivityTree& operator=(ActivityTree&& rhs) = delete;
	~ActivityTree() = default;

// ordinary member functions
private:
	std::set<ActivityNode> const& children(ActivityNode const& p_node) const;
	ActivityInfo const& info(ActivityNode const& p_node) const;

	// TODO MEDIUM PRIORITY params here are cumbersome
	void print
	(	std::ostream& p_os,
		ActivityNode const& p_node,
		unsigned int p_depth,
		bool p_concatenate_to_previous,
		unsigned int p_precision,
		unsigned int p_width,
		unsigned int p_rounding_numerator,
		unsigned int p_rounding_denominator
	) const;
public:
	// TODO MEDIUM PRIORITY params here are cumbersome
	void print
	(	std::ostream& p_os,
		unsigned int p_precision,
		unsigned int p_width,
		unsigned int p_rounding_numerator,
		unsigned int p_rounding_denominator
 	) const;

// member variables
private:
	ActivityNode m_root;
	std::map<ActivityNode, ActivityInfo> m_info_map;
	std::map<ActivityNode, std::set<ActivityNode>> m_inheritance_map;

};  // class ActivityTree

}  // namespace swx

#endif  // GUARD_activity_tree_hpp_4902535711835388
