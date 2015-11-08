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

#include "true_activity_filter.hpp"
#include <boost/test/unit_test.hpp>
#include <string>

using swx::TrueActivityFilter;

namespace test
{

BOOST_AUTO_TEST_CASE(true_activity_filter_matches)
{
    BOOST_CHECK(TrueActivityFilter().matches("hello"));
    BOOST_CHECK(TrueActivityFilter().matches(""));
    BOOST_CHECK(TrueActivityFilter().matches("xyz"));
}

BOOST_AUTO_TEST_CASE(true_activity_filter_replace)
{
    BOOST_CHECK_EQUAL(TrueActivityFilter().replace("hello", "yes"), "yes");
    BOOST_CHECK_EQUAL(TrueActivityFilter().replace("xyz", "yes"), "yes");
    BOOST_CHECK_EQUAL(TrueActivityFilter().replace("", "yes"), "yes");
    BOOST_CHECK_EQUAL(TrueActivityFilter().replace("ksdlkjf", ""), "");
    BOOST_CHECK_EQUAL(TrueActivityFilter().replace("lasdf", ""), "");
}

}  // namespace test
