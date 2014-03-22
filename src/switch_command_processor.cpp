/*
 * Copyright 2014 Matthew Harvey
 */

#include "switch_command_processor.hpp"
#include "command_processor.hpp"
#include "squish.hpp"
#include "time_log.hpp"
#include "time_point.hpp"
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

using std::ostream;
using std::ostringstream;
using std::string;

namespace swx
{

SwitchCommandProcessor::SwitchCommandProcessor(TimeLog& p_time_log):
	m_time_log(p_time_log)
{
}

SwitchCommandProcessor::~SwitchCommandProcessor()
{
}

CommandProcessor::ErrorMessages
SwitchCommandProcessor::do_process
(	Arguments const& p_args,
	ostream& p_ordinary_ostream
)
{
	(void)p_ordinary_ostream;  // ignore param.
	TimePoint const time_point = now();
	Activity const activity(squish(p_args.begin(), p_args.end()));
	m_time_log.append_entry(activity, time_point);
	return ErrorMessages();
}

string
SwitchCommandProcessor::do_provide_help_string
(	string const& p_command_invocation
) const
{
	ostringstream oss;
	oss << p_command_invocation
	    << "\n ARGS     "
		<< "If the stopwatch is already running, stop accruing time onto \n"
		<< "the current activity, and start accruing time on the activity\n"
		<< "named by ARGS.\n\n";
	oss << p_command_invocation << "\nStop the stopwatch.";
	return oss.str();
}

}  // namespace swx
