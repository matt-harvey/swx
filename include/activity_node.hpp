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

#include <string>
#include <vector>

namespace swx
{

/**
 * Represents an activity in a tree structure. "Level" corresponds to
 * distance from leaf, with leaf nodes having a level of 0.
 */
class ActivityNode
{
// special member functions
public:
	ActivityNode(std::string const& p_activity);
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

// ordinary member functions
// TODO document these better
public:
	std::string activity() const;
	std::string marginal_name() const;
	unsigned int num_components() const;
	ActivityNode parent() const;  // undefined behaviour if no parent
	void set_num_components(unsigned int p_num_components);  // can only increase
private:
	static bool is_ancestor_descendant
	(	ActivityNode const& lhs,
		ActivityNode const& rhs
	);

// member variables
private:
	std::vector<std::string> m_components;

};  // class ActivityNode

}  // namespace swx

#endif  // GUARD_activity_node_hpp_591135825771436
