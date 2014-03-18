/*
 * Copyright 2014 Matthew Harvey
 */

#include "version_command_processor.hpp"
#include "command_processor.hpp"
#include "info.hpp"
#include <iostream>
#include <ostream>
#include <string>

using std::endl;
using std::ostream;
using std::string;

namespace swx
{

VersionCommandProcessor::VersionCommandProcessor()
{
}

VersionCommandProcessor::~VersionCommandProcessor()
{
}

int
VersionCommandProcessor::do_process
(	Arguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	(void)p_args;  // ignore param.
	p_ordinary_ostream << Info::application_name()
	                   << " version "
					   << Info::version()
					   << endl;
	return 0;
}

CommandProcessor::ErrorMessages
VersionCommandProcessor::do_validate(Arguments const& p_args)
{
	(void)p_args;  // ignore param.
	return ErrorMessages();
}

string
VersionCommandProcessor::do_provide_help_string
(	string const& p_command_invocation
) const
{
	return p_command_invocation + "        Print version information and exit.";
}

}  // namespace swx
