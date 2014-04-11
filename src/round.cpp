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

#include "round.hpp"
#include <cmath>

using std::ceil;
using std::floor;

namespace swx
{

namespace
{
	double round_whole(double x)
	{
		auto const c = ceil(x);
		auto const f = floor(x);
		return ((c - x) < (x - f))? c: f;
	}

}  // end anonymous namespace

double
round(double x, int num, int den)
{
	x *= den;
	x /= num;
	x = round_whole(x);
	x *= num;
	x /= den;
	return x;
}


}  // namespace swx
