/*
 * Copyright 2014 Matthew Harvey
 */

#include "command_processor.hpp"
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

CommandProcessor::CommandProcessor()
{
}

CommandProcessor::~CommandProcessor()
{
}

int
CommandProcessor::process
(	vector<string> const& p_args,
	ostream& p_ordinary_ostream,
	ostream& p_error_ostream
)
{
	auto const error_messages = do_validate(p_args);
	if (error_messages.empty())
	{
		do_process(p_args, p_ordinary_ostream);
		return 0;  // TODO Should be facility for processing errors here too.
	}
	for (auto const& message: error_messages)
	{
		p_error_ostream << message << endl;
	}
	return error_messages.size();
}

string
CommandProcessor::help_string(string const& p_command_invocation) const
{
	return do_provide_help_string(p_command_invocation);
}

}  // namespace swx
