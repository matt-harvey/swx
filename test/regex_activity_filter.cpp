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

#include "regex_activity_filter.hpp"
#include <boost/test/unit_test.hpp>
#include <string>

using swx::RegexActivityFilter;

namespace test
{

BOOST_AUTO_TEST_CASE(regex_activity_filter_matches)
{
    BOOST_CHECK(RegexActivityFilter("hello").matches("hello"));
    BOOST_CHECK(RegexActivityFilter("hello").matches("hello there"));
    BOOST_CHECK(RegexActivityFilter("hello").matches("there hello"));
    BOOST_CHECK(!RegexActivityFilter("hello").matches("Hello"));
    BOOST_CHECK(RegexActivityFilter("hello there").matches("hello there"));
    BOOST_CHECK(RegexActivityFilter("hell.+there").matches("and hello there friend"));
    BOOST_CHECK(!RegexActivityFilter("hello there").matches("hello"));
    BOOST_CHECK(RegexActivityFilter("").matches(""));
    BOOST_CHECK(!RegexActivityFilter("hello").matches(""));
    BOOST_CHECK(RegexActivityFilter("").matches("hello"));
    BOOST_CHECK(!RegexActivityFilter("^$").matches("hello"));
    BOOST_CHECK(RegexActivityFilter("^h....$").matches("hello"));
    BOOST_CHECK(RegexActivityFilter("goodbye|hello").matches("hello"));
    BOOST_CHECK(RegexActivityFilter("hel.[o-z]").matches("hello"));
    BOOST_CHECK(!RegexActivityFilter("hel.[p-z]").matches("hello"));
}

BOOST_AUTO_TEST_CASE(regex_activity_filter_replace)
{
    BOOST_CHECK_EQUAL(RegexActivityFilter("hello").replace("hello", "yes"), "yes");
    BOOST_CHECK_EQUAL(RegexActivityFilter(" hello").replace("hello", "yes"), "hello");
    BOOST_CHECK_EQUAL(RegexActivityFilter("hey").replace("hey there", "yes"), "yes there");
    BOOST_CHECK_EQUAL(RegexActivityFilter("hey there").replace("there", "yes"), "there");
    BOOST_CHECK_EQUAL(RegexActivityFilter("hey|there").replace("there", "yes"), "yes");
    BOOST_CHECK_EQUAL(RegexActivityFilter("^a b").replace("a b c", "y"), "y c");
    BOOST_CHECK_EQUAL(RegexActivityFilter("w.+").replace("whatever", "yes"), "yes");
}

}  // namespace test
