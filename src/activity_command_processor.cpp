/*
 * Copyright 2014 Matthew Harvey
 */

#include "activity_command_processor.hpp"
#include "interval.hpp"
#include "squish.hpp"
#include "time_log.hpp"
#include <ostream>
#include <string>

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
	ErrorMessages ret;
	string const activity_name = squish(p_args.begin(), p_args.end());
	auto const& activity_ptr = m_time_log.get_activity(activity_name);
	if (activity_ptr == nullptr)
	{
		string msg = "There is no activity named \"" + activity_name + "\".";
		ret.push_back(msg);
		return ret;
	}
	p_ordinary_ostream << activity_ptr->stints();
	return ret;
}

string
ActivityCommandProcessor::do_provide_help_string(string const& p_command_invocation) const
{
	// TODO Implement this.
	(void)p_command_invocation;  // ignore param.
	return "NOT IMPLEMENTED";
}

}  // namespace swx
