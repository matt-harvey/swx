/*
 * Copyright 2014 Matthew Harvey
 */

#include "command_processor.hpp"
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::endl;
using std::ostream;
using std::ostringstream;
using std::setw;
using std::string;
using std::vector;

namespace swx
{

CommandProcessor::HelpLine::HelpLine
(	string const& p_args_descriptor,
	string const& p_usage_descriptor
):
	args_descriptor(p_args_descriptor),
	usage_descriptor(p_usage_descriptor)
{
}

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
	ErrorMessages error_messages = do_validate(p_args);
	if (error_messages.empty())
	{
		error_messages = do_process(p_args, p_ordinary_ostream);
	}
	for (auto const& message: error_messages)
	{
		p_error_ostream << message << endl;
	}
	return 1;
}

CommandProcessor::ErrorMessages
CommandProcessor::do_validate(Arguments const& p_args)
{
	(void)p_args;  // ignore param.
	return ErrorMessages();
}

string
CommandProcessor::usage_descriptor(string const& p_command_word) const
{
	// TODO MEDIUM PRIORITY This should handle wrapping of the right-hand cell to
	// a reasonable width if necessary.
	auto const help_lines = do_get_help_lines();
	typedef string::size_type ColWidth;
	ColWidth command_word_length = p_command_word.length();
	ColWidth left_col_width = command_word_length;
	for (auto const& line: help_lines)
	{
		ColWidth const left_cell_width =
			line.args_descriptor.length() + command_word_length;
		left_col_width =
		(	(left_cell_width > left_col_width)?
			left_cell_width:
			left_col_width
		);
	}
	left_col_width += 3;
	ostringstream oss;
	auto const orig_flags = oss.flags();
	for (auto const& line: help_lines)
	{
		oss << setw(left_col_width)
		    << p_command_word
		    << ' '
		    << line.args_descriptor;
		oss.flags(orig_flags);
		oss << line.usage_descriptor;
	}
	return oss.str();
}

}  // namespace swx
