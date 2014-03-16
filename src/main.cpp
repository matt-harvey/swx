// Copyright 2014 Matthew Harvey

#include "time_conversion.hpp"
#include "time_point.hpp"
#include <iostream>

using std::cout;
using std::endl;

int main()
{
	swx::TimePoint const tp = swx::time_stamp_to_point("2014-01-03 23:09:12");
	cout << swx::time_point_to_stamp(tp) << endl;
	return 0;
}
