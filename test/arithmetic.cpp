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

namespace swx
{
namespace test
{

BOOST_AUTO_TEST_CASE(addition_is_safe_test)
{
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
	// TODO
}

}  // namespace test
}  // namespace swx
