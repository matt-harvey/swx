/*
 * Copyright 2014, 2015 Matthew Harvey
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

#include "csv_utilities.hpp"
#include <cstring>
#include <ostream>

using std::ostream;
using std::string;

namespace swx
{
 
template <>
void
output_csv_cell(ostream& p_os, string const& p_str)
{
    if (p_str.find_first_of(",\"\n\r") == string::npos)
    {
        // no need to quote
        p_os << p_str;
    }
    else
    {
        // need to quote and escape
        p_os << '"';
        for (auto const c: p_str)
        {
            p_os << c;
            if (c == '"') p_os << c;
        }
        p_os << '"';
    }
}

}  // namespace swx
