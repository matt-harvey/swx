/*
 * Copyright 2014 Matthew Harvey
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
