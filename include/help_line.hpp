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

#ifndef GUARD_help_line_hpp_07384218837779118
#define GUARD_help_line_hpp_07384218837779118

#include <string>

namespace swx
{

/**
 * Represents a line of help information for presentation to user,
 * describing a particular string of arguments that might be passed to
 * a command or command option, together with a description of how the
 * command or option is used in conjunction with that string of arguments.
 */
class HelpLine
{
// special member functions
public:
    HelpLine
    (   std::string const& p_usage_descriptor,
        std::string const& p_args_descriptor = std::string()
    );

    HelpLine(char const* p_usage_descriptor);

// accessors
public:
    std::string usage_descriptor() const;
    std::string args_descriptor() const;

// data members
private:
    std::string m_usage_descriptor;
    std::string m_args_descriptor;

};  // class HelpLine

}  // namespace swx

#endif  // GUARD_help_line_hpp_07384218837779118
