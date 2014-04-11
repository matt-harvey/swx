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

#ifndef GUARD_command_hpp_08439188501070807
#define GUARD_command_hpp_08439188501070807

#include <ostream>
#include <string>
#include <vector>

namespace swx
{

/**
 * @todo Find a more unified way to generate the help messages for
 * each subclass (to ensure they're all in the same format).
 */
class Command
{
// nested types and typedefs
protected:
	typedef std::vector<std::string> Arguments;
	typedef std::vector<std::string> ErrorMessages;

	struct HelpLine
	{
	public:
		friend class Command;
		HelpLine
		(	std::string const& p_args_descriptor,
			std::string const& p_usage_descriptor
		);
	private:
		std::string args_descriptor;
		std::string usage_descriptor;
	};

// special member functions
public:
	Command
	(	std::string const& p_command_word,
		std::vector<std::string> const& p_aliases
	);
	Command(Command const& rhs) = delete;
	Command(Command&& rhs) = delete;
	Command& operator=(Command const& rhs) = delete;
	Command& operator=(Command&& rhs) = delete;
	virtual ~Command();

// ordinary member functions
public:
	int process
	(	std::vector<std::string> const& p_args,
		std::ostream& p_ordinary_ostream,
		std::ostream& p_error_ostream
	);

	std::string usage_descriptor() const;
	std::string const& command_word() const;
	std::vector<std::string> const& aliases() const;

// virtual functions
private:

	virtual ErrorMessages do_process
	(	Arguments const& p_args,
		std::ostream& p_ordinary_ostream
	) = 0;
	
	virtual std::vector<HelpLine> do_get_help_lines() const = 0;

// member variables
private:
	std::string const m_command_word;
	std::vector<std::string> const m_aliases;

};  // class Command

}  // namespace swx

#endif  // GUARD_command_hpp_08439188501070807
