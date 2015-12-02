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

#include "help_line.hpp"
#include <string>

using std::string;

namespace swx
{

HelpLine::HelpLine(string const& p_usage_descriptor, string const& p_args_descriptor):
    m_usage_descriptor(p_usage_descriptor),
    m_args_descriptor(p_args_descriptor)
{
}

HelpLine::HelpLine(char const* p_usage_descriptor): m_usage_descriptor(p_usage_descriptor)
{
}

string
HelpLine::usage_descriptor() const
{
    return m_usage_descriptor;
}

string
HelpLine::args_descriptor() const
{
    return m_args_descriptor;
}

}  // namespace swx
