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

#include "csv_row.hpp"
#include <boost/test/unit_test.hpp>
#include <sstream>

using std::ostringstream;
using swx::CsvRow;

namespace test
{

BOOST_AUTO_TEST_CASE(csv_row_str_and_member_output_operator)
{
    // empty row
    CsvRow row;
    BOOST_CHECK_EQUAL(row.str(), "");

    // string
    CsvRow row0;
    row0 << "Hello there";
    BOOST_CHECK_EQUAL(row0.str(), "Hello there");
    // returns the CsvRow itself
    BOOST_CHECK_EQUAL(&(row0 << "abc"), &row0);

    // string with quotes
    CsvRow row1;
    row1 << "Hello \"Doctor\"!";
    BOOST_CHECK_EQUAL(row1.str(), "\"Hello \"\"Doctor\"\"!\"");

    // string with comma
    CsvRow row2;
    row2 << "Hello, there";
    BOOST_CHECK_EQUAL(row2.str(), "\"Hello, there\"");

    // string with quote and comma
    CsvRow row3;
    row3 << "Hello, \"there\"";
    BOOST_CHECK_EQUAL(row3.str(), "\"Hello, \"\"there\"\"\"");

    // string with newline
    CsvRow row4;
    row4 << "Hello\nthere";
    BOOST_CHECK_EQUAL(row4.str(), "\"Hello\nthere\"");

    // string with carriage return
    CsvRow row5;
    row5 << "Hello\rthere";
    BOOST_CHECK_EQUAL(row5.str(), "\"Hello\rthere\"");

    // integer
    CsvRow row6;
    row6 << 300;
    BOOST_CHECK_EQUAL(row6.str(), "300");

    // negative double
    CsvRow row7;
    row7 << -33.9;
    BOOST_CHECK_EQUAL(row7.str(), "-33.9");

    // multiple cells
    CsvRow row8;
    row8 << "Hello" << 33.905 << "\"Yes indeed\"" << -5 << "Interesting, \"hey\"?";
    BOOST_CHECK_EQUAL
    (   row8.str(),
        "Hello,33.905,\"\"\"Yes indeed\"\"\",-5,\"Interesting, \"\"hey\"\"?\""
    );
}

BOOST_AUTO_TEST_CASE(csv_row_output_operator)
{
    // empty row
    CsvRow row;
    ostringstream oss;
    oss << row;
    BOOST_CHECK_EQUAL(oss.str(), "\n");

    // string
    CsvRow row0;
    row0 << "Hello there";
    ostringstream oss0;
    oss0 << row0;
    BOOST_CHECK_EQUAL(oss0.str(), "Hello there\n");

    // string with newline
    CsvRow row1;
    row1 << "Hello\nthere";
    ostringstream oss1;
    oss1 << row1;
    BOOST_CHECK_EQUAL(oss1.str(), "\"Hello\nthere\"\n");

    // multiple cells
    CsvRow row2;
    row2 << "Hello" << 33.905 << "\"Yes indeed\"" << -5 << "Interesting, \"hey\"?";
    ostringstream oss2;
    oss2 << row2;
    BOOST_CHECK_EQUAL
    (   oss2.str(),
        "Hello,33.905,\"\"\"Yes indeed\"\"\",-5,\"Interesting, \"\"hey\"\"?\"\n"
    );
}
}  // namespace test
