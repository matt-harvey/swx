/*
 * Copyright 2014 Matthew Harvey
 */

#include "switch_command_processor.hpp"
#include "command_processor.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include "time_point.hpp"
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::move;
using std::ostream;
using std::ostringstream;
using std::string;
using std::vector;

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
	string const activity_name(squish(p_args.begin(), p_args.end()));
	m_time_log.append_entry(move(activity_name), time_point);
	return ErrorMessages();
}

vector<CommandProcessor::HelpLine>
SwitchCommandProcessor::do_get_help_lines() const
{
	HelpLine const switching_line
	(	"ACTIVITY",
		"Start accruing time to ACTIVITY, and stop accruing time to the "
			"current activity (if any)"
	);
	HelpLine const stopping_line
	(	"",
		"Stop accruing time to any activity"
	);
	return vector<HelpLine>{switching_line, stopping_line};
}

}  // namespace swx
