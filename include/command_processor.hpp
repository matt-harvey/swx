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

	struct HelpLine
	{
	public:
		friend class CommandProcessor;
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
	CommandProcessor
	(	std::string const& p_command_word,
		std::vector<std::string> const& p_aliases
	);
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

	std::string usage_descriptor() const;
	std::string const& command_word() const;
	std::vector<std::string> const& aliases() const;

// virtual functions
private:

	virtual ErrorMessages do_process
	(	Arguments const& p_args,
		std::ostream& p_ordinary_ostream
	) = 0;

	virtual ErrorMessages do_validate(Arguments const& p_args);
	
	virtual std::vector<HelpLine> do_get_help_lines() const = 0;

// member variables
private:
	std::string const m_command_word;
	std::vector<std::string> const m_aliases;

};  // class CommandProcessor

}  // namespace swx

#endif  // GUARD_command_processor_hpp_08439188501070807
