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

#ifndef GUARD_csv_utilities_hpp_21343681784972401
#define GUARD_csv_utilities_hpp_21343681784972401

#include "stream_utilities.hpp"
#include <ostream>
#include <sstream>
#include <string>

namespace swx
{

template <typename T>
void output_csv_cell(std::ostream& p_os, T const& p_contents);

template <>
void output_csv_cell(std::ostream& p_os, std::string const& p_contents);

template <typename Head>
void output_csv_row(std::ostream& p_os, Head const& p_head);

template <typename Head, typename ... Rest>
void output_csv_row
(	std::ostream& p_os,
	Head const& p_head,
	Rest const& ... p_rest
);

// TEMPLATE IMPLEMENTATIONS

template <typename T>
void
output_csv_cell(std::ostream& p_os, T const& p_contents)
{
	std::ostringstream oss;
	enable_exceptions(oss);
	oss << p_contents;
	output_csv_cell(p_os, oss.str());
	return;
}

template <typename Head>
void
output_csv_row(std::ostream& p_os, Head const& p_head)
{
	output_csv_cell(p_os, p_head);
	p_os << std::endl;
	return;
}

template <typename Head, typename ... Rest>
void
output_csv_row(std::ostream& p_os, Head const& p_head, Rest const& ... p_rest)
{
	output_csv_cell(p_os, p_head);
	p_os << ',';
	output_csv_row(p_os, p_rest ...);
	return;
}

}  // namespace swx

#endif  // GUARD_csv_utilities_hpp_21343681784972401
