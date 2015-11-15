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

#ifndef GUARD_csv_row_hpp_40090279206675605
#define GUARD_csv_row_hpp_40090279206675605

#include <ostream>
#include <string>
#include <sstream>
#include "stream_utilities.hpp"

namespace swx
{

class CsvRow
{
// special member functions
public:
    CsvRow();
    CsvRow(CsvRow const& rhs) = delete;
    CsvRow(CsvRow&& rhs) = delete;
    CsvRow& operator=(CsvRow const& rhs) = delete;
    CsvRow& operator=(CsvRow&& rhs) = delete;
    ~CsvRow() = default;

// ordinary member functions
public:
    std::string str() const;
    
// member operators
public:
    template <typename T> CsvRow& operator<<(T const& p_contents);

// data members
private:
    bool m_started = false;
    std::ostringstream m_contents;

};  // class CsvRow


// NON-MEMBER FUNCTIONS

std::ostream& operator<<(std::ostream& p_os, CsvRow const& p_csv_row);


// FUNCTION TEMPLATE IMPLEMENTATIONS

template <typename T>
CsvRow&
CsvRow::operator<<(T const& p_contents)
{
    std::ostringstream oss;
    enable_exceptions(oss);
    oss << p_contents;
    return *this << oss.str();
}

}  // namespace swx

#endif  // GUARD_csv_row_hpp_40090279206675605
