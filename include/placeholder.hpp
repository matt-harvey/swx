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

#ifndef GUARD_placeholder_hpp_31044386248590605
#define GUARD_placeholder_hpp_31044386248590605

#include "time_log_fwd.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace swx
{

/**
 * Takes \e p_components, expands any that are placeholders, in the
 * context of \e p_time_log, and returns the resulting activity name,
 * with any additional whitespace removed (other than a single space
 * between each activity component).
 */
std::string expand_placeholders
(   std::vector<std::string> const& p_components,
    TimeLog& p_time_log
);

/**
 * Writes to \e p_os text in two columns explaining the use of placeholders.
 * The left column of the text will take up no less than \e p_margin characters,
 * and the whole text, both columns included, will take up no more than \e
 * p_width characters.
 */
void write_placeholder_help
(   std::ostream& p_os,
    std::string::size_type p_margin = 0,
    std::string::size_type p_width = 80
);

}  // namespace swx

#endif  // GUARD_placeholder_hpp_31044386248590605
