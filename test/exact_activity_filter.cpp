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

#include "exact_activity_filter.hpp"
#include <boost/test/unit_test.hpp>
#include <string>

using swx::ExactActivityFilter;

namespace test
{

BOOST_AUTO_TEST_CASE(exact_activity_filter_matches)
{
    BOOST_CHECK(ExactActivityFilter("hello").matches("hello"));
    BOOST_CHECK(!ExactActivityFilter("hello").matches("hello there"));
    BOOST_CHECK(!ExactActivityFilter("hello").matches("there hello"));
    BOOST_CHECK(!ExactActivityFilter("hello").matches(" hello"));
    BOOST_CHECK(!ExactActivityFilter("hello").matches("Hello"));
    BOOST_CHECK(ExactActivityFilter("hello there").matches("hello there"));
}

BOOST_AUTO_TEST_CASE(exact_activity_filter_replace)
{
    BOOST_CHECK_EQUAL(ExactActivityFilter("hello").replace("hello", "yes"), "yes");
    BOOST_CHECK_EQUAL(ExactActivityFilter(" hello").replace("hello", "yes"), "hello");
    BOOST_CHECK_EQUAL(ExactActivityFilter("hey").replace("hey there", "yes"), "hey there");
    BOOST_CHECK_EQUAL(ExactActivityFilter("hey there").replace("there", "yes"), "there");
    BOOST_CHECK_EQUAL(ExactActivityFilter("").replace("", "yes"), "yes");
    BOOST_CHECK_EQUAL(ExactActivityFilter("yes").replace("", "yes"), "");
}

}  // namespace test
