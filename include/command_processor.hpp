/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_command_processor_hpp_08439188501070807
#define GUARD_command_processor_hpp_08439188501070807

#include <string>
#include <vector>

namespace swx
{

class CommandProcessor
{
public:
	CommandProcessor();
	CommandProcessor(CommandProcessor const& rhs) = delete;
	CommandProcessor(CommandProcessor&& rhs) = delete;
	CommandProcessor& operator=(CommandProcessor const& rhs) = delete;
	CommandProcessor& operator=(CommandProcessor&& rhs) = delete;
	virtual ~CommandProcessor();

	void process(std::vector<std::string> const& p_args);

private:
	virtual void do_process(std::vector<std::string> const& p_args) = 0;

};  // class CommandProcessor

}  // namespace swx

#endif  // GUARD_command_processor_hpp_08439188501070807
