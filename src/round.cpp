/*
 * Copyright 2014 Matthew Harvey
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
