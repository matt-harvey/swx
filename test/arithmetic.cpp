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

#include "arithmetic.hpp"
#include <boost/test/unit_test.hpp>
#include <limits>

using std::numeric_limits;

namespace test
{

BOOST_AUTO_TEST_CASE(addition_is_safe_test)
{
    using swx::addition_is_safe;

    unsigned long long const ullmax = numeric_limits<unsigned long long>::max();
    
    // test reaction to safe operations
    BOOST_CHECK(addition_is_safe(0ULL, 1ULL));
    BOOST_CHECK(addition_is_safe(0ULL, -1ULL));
    BOOST_CHECK(addition_is_safe(ullmax / 2ULL, ullmax / 2ULL));
    BOOST_CHECK(addition_is_safe(ullmax - 10ULL, 10ULL));

    // test reaction to unsafe operations
    BOOST_CHECK(!addition_is_safe(ullmax - 3, 10ULL));
    BOOST_CHECK(!addition_is_safe(ullmax, 1ULL));
}

BOOST_AUTO_TEST_CASE(round)
{
    using swx::round;

    BOOST_CHECK_CLOSE(round(1.0, 1, 1), 1.0, 0.0001);
    BOOST_CHECK_CLOSE(round(1.0, 1, 2), 1.0, 0.0001);
    BOOST_CHECK_CLOSE(round(1.0, 2, 1), 2.0, 0.0001);
    BOOST_CHECK_CLOSE(round(-1.0, 2, 1), -2.0, 0.0001);
    BOOST_CHECK_CLOSE(round(0.5, 1, 2), 0.5, 0.0001);
    BOOST_CHECK_CLOSE(round(0.4, 1, 2), 0.5, 0.0001);
    BOOST_CHECK_CLOSE(round(0.12, 1, 10), 0.1, 0.0001);
    BOOST_CHECK_CLOSE(round(-0.04, 1, 10), 0.0, 0.0001);
    BOOST_CHECK_CLOSE(round(-0.24, 1, 10), -0.2, 0.0001);
    BOOST_CHECK_CLOSE(round(-0.24, 1, 4), -0.25, 0.0001);
    BOOST_CHECK_CLOSE(round(-600.124, 1, 4), -600.0, 0.0001);
    BOOST_CHECK_CLOSE(round(-600.5, 1, 4), -600.5, 0.0001);
    BOOST_CHECK_CLOSE(round(3.64, 1, 4), 3.75, 0.0001);
    BOOST_CHECK_CLOSE(round(0.8, 2, 5), 0.8, 0.0001);
    BOOST_CHECK_CLOSE(round(0.9, 2, 5), 0.8, 0.0001);
}

}  // namespace test
