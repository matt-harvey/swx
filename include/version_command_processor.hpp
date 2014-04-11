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

#ifndef GUARD_version_command_processor_hpp_12242928944641225
#define GUARD_version_command_processor_hpp_12242928944641225

#include "command_processor.hpp"
#include <ostream>
#include <string>

namespace swx
{

class VersionCommandProcessor: public CommandProcessor
{
// special member functions
public:
	VersionCommandProcessor
	(	std::string const& p_command_word,
		std::vector<std::string> const& p_aliases
	);
	VersionCommandProcessor(VersionCommandProcessor const& rhs) = delete;
	VersionCommandProcessor(VersionCommandProcessor&& rhs) = delete;
	VersionCommandProcessor& operator=(VersionCommandProcessor const& rhs) = delete;
	VersionCommandProcessor& operator=(VersionCommandProcessor&& rhs) = delete;
	virtual ~VersionCommandProcessor();

// inherited virtual functions
private:
	virtual ErrorMessages do_process
	(	Arguments const& p_args,
		std::ostream& p_ordinary_ostream
	) override;

	virtual ErrorMessages do_validate(Arguments const& p_args) override;

	std::vector<HelpLine> do_get_help_lines() const override;

};  // class VersionCommandProcessor

}  // namespace swx

#endif  // GUARD_version_command_processor_hpp_12242928944641225
