/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_command_processor_hpp_08439188501070807
#define GUARD_command_processor_hpp_08439188501070807

#include <ostream>
#include <string>
#include <vector>

namespace swx
{

/**
 * @todo Find a more unified way to generate the help messages for
 * each subclass (to ensure they're all in the same format).
 */
class CommandProcessor
{
// nested types and typedefs
protected:
	typedef std::vector<std::string> Arguments;
	typedef std::vector<std::string> ErrorMessages;

// special member functions
public:
	CommandProcessor();
	CommandProcessor(CommandProcessor const& rhs) = delete;
	CommandProcessor(CommandProcessor&& rhs) = delete;
	CommandProcessor& operator=(CommandProcessor const& rhs) = delete;
	CommandProcessor& operator=(CommandProcessor&& rhs) = delete;
	virtual ~CommandProcessor();

// ordinary member functions
public:
	int process
	(	std::vector<std::string> const& p_args,
		std::ostream& p_ordinary_ostream,
		std::ostream& p_error_ostream
	);

	std::string help_string
	(	std::string const& p_command_invocation
	) const;

// virtual functions
private:

	virtual ErrorMessages do_process
	(	Arguments const& p_args,
		std::ostream& p_ordinary_ostream
	) = 0;

	virtual ErrorMessages do_validate(Arguments const& p_args);
	
	virtual std::string do_provide_help_string
	(	std::string const& p_command_invocation
	) const = 0;

};  // class CommandProcessor

}  // namespace swx

#endif  // GUARD_command_processor_hpp_08439188501070807
