/*
 * Copyright 2014 Matthew Harvey
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

#include "stint.hpp"
#include "config.hpp"
#include "interval.hpp"
#include "round.hpp"
#include "stream_flag_guard.hpp"
#include <cassert>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>
#include <map>
#include <string>
#include <vector>

using std::endl;
using std::fixed;
using std::ios_base;
using std::left;
using std::map;
using std::ostream;
using std::right;
using std::setprecision;
using std::setw;
using std::string;
using std::vector;

namespace swx
{

Stint::Stint(string const& p_activity, Interval const& p_interval):
	m_activity(p_activity),
	m_interval(p_interval)
{
}

string const&
Stint::activity() const
{
	return m_activity;
}

Interval
Stint::interval() const
{
	return m_interval;
}

}  // namespace swx
