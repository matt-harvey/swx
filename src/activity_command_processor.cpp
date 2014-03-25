/*
 * Copyright 2014 Matthew Harvey
 */

#include "activity_command_processor.hpp"
#include "interval.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include <ostream>
#include <string>
#include <utility>

using std::move;
using std::string;

namespace swx
{

ActivityCommandProcessor::ActivityCommandProcessor(TimeLog& p_time_log):
	m_time_log(p_time_log)
{
}

ActivityCommandProcessor::~ActivityCommandProcessor()
{
}

CommandProcessor::ErrorMessages
ActivityCommandProcessor::do_process
(	Arguments const& p_args,
	std::ostream& p_ordinary_ostream
)
{
	string const activity_name = squish(p_args.begin(), p_args.end());
	p_ordinary_ostream <<
		m_time_log.get_intervals_by_activity_name(move(activity_name));
	return ErrorMessages();
}

string
ActivityCommandProcessor::do_provide_help_string(string const& p_command_invocation) const
{
	// TODO Implement this.
	(void)p_command_invocation;  // ignore param.
	return "NOT IMPLEMENTED";
}

}  // namespace swx
