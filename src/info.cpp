/*
 * Copyright 2014 Matthew Harvey
 */

#include "info.hpp"
#include <sstream>
#include <string>

using std::string;
using std::ostringstream;

namespace swx
{

string
Info::application_name()
{
	return SWX_APPLICATION_NAME;
}

string
Info::version()
{
	ostringstream oss;
	oss << SWX_VERSION_MAJOR << '.'
	    << SWX_VERSION_MINOR << '.'
		<< SWX_VERSION_PATCH;
	return oss.str();
}

}  // namespace swx
