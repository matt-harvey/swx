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

#ifndef GUARD_activity_node_hpp_591135825771436
#define GUARD_activity_node_hpp_591135825771436

#include "activity_stats.hpp"
#include <map>
#include <string>
#include <vector>

namespace swx
{

/**
 * Represents a node in a tree structure of nested activities.
 * Each ActivityNode has one or more components, corresponding to the
 * single-space-separated components of an activity.
 */
class ActivityNode
{
// special member functions
public:

	explicit ActivityNode(std::string const& p_activity);

	/**
	 * Initializes the node using \e p_activity then resizes it to
	 * \e p_depth, either by adding empty components, or by deleting
	 * components from the end, until \e p_depth is reached.
	 */
	ActivityNode(std::string const& p_activity, unsigned int p_depth);

private:
	ActivityNode
	(	std::vector<std::string>::const_iterator const& p_begin,
		std::vector<std::string>::const_iterator const& p_end
	);
public:
	ActivityNode(ActivityNode const& rhs) = default;
	ActivityNode(ActivityNode&& rhs) = default;
	ActivityNode& operator=(ActivityNode const& rhs) = default;
	ActivityNode& operator=(ActivityNode&& rhs) = default;
	~ActivityNode() = default;

// operators
public:
	bool operator<(ActivityNode const& rhs) const;
	bool operator==(ActivityNode const& rhs) const;
	bool operator!=(ActivityNode const& rhs) const;

// ordinary member functions
public:
	/**
	 * @returns the full name of the node's activity
	 */
	std::string activity() const;

	/**
	 * @returns the name of the last component of the activity.
	 */
	std::string marginal_name() const;

	/**
	 * @returns an ActivityNode corresponding to the "parent" of the
	 * activity. If there is no parent (which will be the case if and only
	 * if the node has no components), behaviour is undefined.
	 */
	ActivityNode parent() const;

// member variables
private:
	std::vector<std::string> m_components;

};  // class ActivityNode

}  // namespace swx

#endif  // GUARD_activity_node_hpp_591135825771436
