/*
 * Copyright 2014 Matthew Harvey
 */

#include "version_command_processor.hpp"
#include "command_processor.hpp"
#include "info.hpp"
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using std::endl;
using std::ostream;
using std::string;
using std::vector;

namespace swx
{

VersionCommandProcessor::VersionCommandProcessor
(	string const& p_command_word,
	vector<string> const& p_aliases
):
	CommandProcessor(p_command_word, p_aliases)
{
}

VersionCommandProcessor::~VersionCommandProcessor()
{
}

CommandProcessor::ErrorMessages
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
	return ErrorMessages();
}

CommandProcessor::ErrorMessages
VersionCommandProcessor::do_validate(Arguments const& p_args)
{
	(void)p_args;  // ignore param.
	return ErrorMessages();
}

vector<CommandProcessor::HelpLine>
VersionCommandProcessor::do_get_help_lines() const
{
	HelpLine const basic_usage_help_line
	(	"",
		"Print version information and exit"
	);
	return vector<HelpLine>{basic_usage_help_line};
}

}  // namespace swx
