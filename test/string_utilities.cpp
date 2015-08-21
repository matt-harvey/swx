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

#include "string_utilities.hpp"
#include <boost/test/unit_test.hpp>
#include <iterator>
#include <string>
#include <vector>

using std::begin;
using std::end;
using std::string;
using std::vector;

namespace test
{

BOOST_AUTO_TEST_CASE(trim)
{
    using swx::trim;

    BOOST_CHECK_EQUAL(trim("hello there"), "hello there");
    BOOST_CHECK_EQUAL(trim(" hello there"), "hello there");
    BOOST_CHECK_EQUAL(trim("hello there "), "hello there");
    BOOST_CHECK_EQUAL(trim(" hello ."), "hello .");
    BOOST_CHECK_EQUAL(trim("9hello "), "9hello");
    BOOST_CHECK_EQUAL(trim("\n\n\nhello "), "hello");
    BOOST_CHECK_EQUAL(trim(" \nhello there\t "), "hello there");
}

BOOST_AUTO_TEST_CASE(squish)
{
    using swx::squish;

    auto const strings = { " hello  there\n", "everyone", "what's  \t", " happening? " };

    BOOST_CHECK_EQUAL
    (   squish(begin(strings), end(strings)),
        "hello  there everyone what's happening?"
    );
}

BOOST_AUTO_TEST_CASE(split)
{
    using swx::split;
    
    string const str0("hello  there, everyone,, what's happening?,,");
    vector<string> const vec0{"hello  there", " everyone", "", " what's happening?", "", ""};
    BOOST_CHECK(split(str0, ',') == vec0);

    string const str1(",");
    vector<string> const vec1{"", ""};
    BOOST_CHECK(split(str1, ',') == vec1);

    string const str2("");
    vector<string> const vec2{};
    BOOST_CHECK(split(str2, ',') == vec2);

    string const str3(",hello,there");
    vector<string> const vec3{"", "hello", "there"};
    BOOST_CHECK(split(str3, ',') == vec3);

    string const str4("hello there");
    vector<string> const vec4{"hello", "there"};
    BOOST_CHECK(split(str4) == vec4);

    string const str5("yeah");
    vector<string> const vec5{"yeah"};
    BOOST_CHECK(split(str5, '!') == vec5);
}

}  // namespace test
