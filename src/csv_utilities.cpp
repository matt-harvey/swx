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

#include "csv_utilities.hpp"
#include <ostream>
#include <string>

using std::ostream;
using std::string;

namespace swx
{
    
template <>
void
output_csv_cell(ostream& p_os, string const& p_str)
{
    auto use_quotes = false;
    for (auto it = p_str.begin(); !use_quotes && (it != p_str.end()); ++it)
    {
        switch (*it)
        {
        case ',': case '"': case '\n': case '\r':
            use_quotes = true;  
        default:
            ;  // do nothing
        }
    }
    if (use_quotes)
    {
        p_os << '"';
        for (char c: p_str)
        {
            if (c == '"') p_os << "\"\"";
            else p_os << c;
        }
        p_os << '"';
    }
    else
    {
        p_os << p_str;
    }
    return;
}

}  // namespace swx
