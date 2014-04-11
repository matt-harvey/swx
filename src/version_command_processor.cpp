/*
 * Copyright 2014 Matthew Harvey
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
