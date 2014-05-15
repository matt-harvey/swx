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

#ifndef GUARD_stream_utilities_hpp_5786451262322465
#define GUARD_stream_utilities_hpp_5786451262322465

#include <ios>

namespace swx
{

template <typename charT, typename traits>
void enable_exceptions(std::basic_ios<charT, traits>& p_stream);

// FUNCTION TEMPLATE IMPLEMENTATIONS

template <typename charT, typename traits>
void
enable_exceptions(std::basic_ios<charT, traits>& p_stream)
{
	p_stream.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	return;
}

}  // namespace swx

#endif  // GUARD_stream_utilities_hpp_5786451262322465
