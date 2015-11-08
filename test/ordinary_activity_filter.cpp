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

#include "ordinary_activity_filter.hpp"
#include <boost/test/unit_test.hpp>
#include <string>

using swx::OrdinaryActivityFilter;

namespace test
{

BOOST_AUTO_TEST_CASE(ordinary_activity_filter_matches)
{
    BOOST_CHECK(OrdinaryActivityFilter("hello").matches("hello"));
    BOOST_CHECK(OrdinaryActivityFilter("hello").matches("hello there"));
    BOOST_CHECK(!OrdinaryActivityFilter("hello").matches("there hello"));
    BOOST_CHECK(!OrdinaryActivityFilter("hello").matches(" hello"));
    BOOST_CHECK(!OrdinaryActivityFilter("hello").matches("Hello"));
    BOOST_CHECK(OrdinaryActivityFilter("hello there").matches("hello there"));
    BOOST_CHECK(OrdinaryActivityFilter("hello there").matches("hello there friend"));
    BOOST_CHECK(!OrdinaryActivityFilter("hello there").matches("hello"));
    BOOST_CHECK(OrdinaryActivityFilter("").matches(""));
    BOOST_CHECK(!OrdinaryActivityFilter("hello").matches(""));
    BOOST_CHECK(!OrdinaryActivityFilter("").matches("hello"));
}

BOOST_AUTO_TEST_CASE(ordinary_activity_filter_replace)
{
    BOOST_CHECK_EQUAL(OrdinaryActivityFilter("hello").replace("hello", "yes"), "yes");
    BOOST_CHECK_EQUAL(OrdinaryActivityFilter(" hello").replace("hello", "yes"), "hello");
    BOOST_CHECK_EQUAL(OrdinaryActivityFilter("hey").replace("hey there", "yes"), "yes there");
    BOOST_CHECK_EQUAL(OrdinaryActivityFilter("hey there").replace("there", "yes"), "there");
    BOOST_CHECK_EQUAL(OrdinaryActivityFilter("a b").replace("a b c", "y"), "y c");
    BOOST_CHECK_EQUAL(OrdinaryActivityFilter("xyz").replace("x b c", "y"), "x b c");
    BOOST_CHECK_EQUAL(OrdinaryActivityFilter("").replace("", "yes"), "yes");
    BOOST_CHECK_EQUAL(OrdinaryActivityFilter("yes").replace("", "yes"), "");
    BOOST_CHECK_EQUAL(OrdinaryActivityFilter("").replace("yes", ""), "yes");
}

}  // namespace test
