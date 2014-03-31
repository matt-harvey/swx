/*
 * Copyright 2014 Matthew Harvey
 */

#include "command_processor.hpp"
#include "info.hpp"
#include "stream_flag_guard.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::endl;
using std::left;
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

CommandProcessor::CommandProcessor
(	string const& p_command_word,
	vector<string> const& p_aliases
):
	m_command_word(p_command_word),
	m_aliases(p_aliases)
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
	if (error_messages.empty())
	{
		return 0;
	}
	assert (error_messages.size() > 0);
	return 1;
}

CommandProcessor::ErrorMessages
CommandProcessor::do_validate(Arguments const& p_args)
{
	(void)p_args;  // ignore param.
	return ErrorMessages();
}

string
CommandProcessor::usage_descriptor() const
{
	// TODO MEDIUM PRIORITY This should handle wrapping of the right-hand cell to
	// a reasonable width if necessary.
	auto const help_lines = do_get_help_lines();
	typedef string::size_type ColWidth;
	ColWidth command_word_length = m_command_word.length();
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
	left_col_width +=
		Info::application_name().length() + 1 + m_command_word.length() + 2;
	ostringstream oss;
	for (auto const& line: help_lines)
	{
		StreamFlagGuard guard(oss);
		oss << setw(left_col_width)
		    << left
		    << Info::application_name() + ' ' + m_command_word + ' ' +
			     line.args_descriptor
		    << "  ";
		guard.reset();
		oss << line.usage_descriptor
		    << '\n';
	}
	if (!m_aliases.empty())
	{
		oss << "\nAliased as: ";
		auto it = m_aliases.begin();
		oss << *it;
		for (++it; it != m_aliases.end(); ++it)
		{
			oss << ", " << *it;	
		}
	}
	return oss.str();
}

string const&
CommandProcessor::command_word() const
{
	return m_command_word;
}

vector<string> const&
CommandProcessor::aliases() const
{
	return m_aliases;
}

}  // namespace swx
