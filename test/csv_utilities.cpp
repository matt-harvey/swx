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

#include "csv_utilities.hpp"
#include <boost/test/unit_test.hpp>
#include <sstream>

using std::ostringstream;

namespace test
{

BOOST_AUTO_TEST_CASE(output_csv_cell)
{
    using swx::output_csv_cell;

    // string
    ostringstream oss0;
    output_csv_cell(oss0, "Hello there");
    BOOST_CHECK_EQUAL(oss0.str(), "\"Hello there\"");

    // string with quotes
    ostringstream oss1;
    output_csv_cell(oss1, "Hello \"Doctor\"!");
    BOOST_CHECK_EQUAL(oss1.str(), "\"Hello \"\"Doctor\"\"!\"");

    // integer
    ostringstream oss2;
    output_csv_cell(oss2, 300);
    BOOST_CHECK_EQUAL(oss2.str(), "\"300\"");

    // negative double
    ostringstream oss3;
    output_csv_cell(oss3, -33.9);
    BOOST_CHECK_EQUAL(oss3.str(), "\"-33.9\"");
}

BOOST_AUTO_TEST_CASE(output_csv_row)
{
    using swx::output_csv_row;

    // one cell, string
    ostringstream oss0;
    output_csv_row(oss0, "Hello there");
    BOOST_CHECK_EQUAL(oss0.str(), "\"Hello there\"\n");

    // one cell, double
    ostringstream oss1;
    output_csv_row(oss1, 33.9);
    BOOST_CHECK_EQUAL(oss1.str(), "\"33.9\"\n");

    // multiple cells
    ostringstream oss2;
    output_csv_row(oss2, "Hello", 33.905, "\"Yes indeed\"", -5, "Interesting \"hey\"?");
    BOOST_CHECK_EQUAL
    (   oss2.str(),
        "\"Hello\",\"33.905\",\"\"\"Yes indeed\"\"\",\"-5\",\"Interesting \"\"hey\"\"?\"\n"
    );
}

}  // namespace test
