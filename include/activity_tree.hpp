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
#include <functional>
#include <map>
#include <set>
#include <ostream>
#include <string>

namespace swx
{

// begin forward declarations

struct ActivityStats;

// end forward declarations

class ActivityTree
{
// nested types
private:
    struct ActivityData
    {
        explicit ActivityData
        (   ActivityStats const& p_stats = ActivityStats(),
            std::set<ActivityNode> const& p_children = std::set<ActivityNode>()
        );
        ActivityStats stats;
        std::set<ActivityNode> children;
    };

public:
    typedef std::function
    <    void
        (   std::ostream& p_os,
            unsigned int p_depth,
            std::string const& p_node_label,
            ActivityStats const& p_stats
        )
    >
    PrintNode;


// special member functions
public:
    explicit ActivityTree(std::map<std::string, ActivityStats> const& p_stats);
    ActivityTree(ActivityTree const& rhs) = delete;
    ActivityTree(ActivityTree&& rhs) = delete;
    ActivityTree& operator=(ActivityTree const& rhs) = delete;
    ActivityTree& operator=(ActivityTree&& rhs) = delete;
    ~ActivityTree() = default;

// ordinary member functions
private:
    void print
    (   std::ostream& p_os,
        ActivityNode const& p_node,
        std::string const& p_label,
        unsigned int p_depth,
        PrintNode const& p_print_node
    ) const;
public:
    void print(std::ostream& p_os, PrintNode const& p_print_node) const;

// member variables
private:
    ActivityNode m_root;
    std::map<ActivityNode, ActivityData> m_map;

};  // class ActivityTree

}  // namespace swx

#endif  // GUARD_activity_tree_hpp_4902535711835388
