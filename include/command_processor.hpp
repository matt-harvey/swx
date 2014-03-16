/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_command_processor_hpp_08439188501070807
#define GUARD_command_processor_hpp_08439188501070807

namespace swx
{

class CommandProcessor
{
public:
	CommandProcessor() = default;
	CommandProcessor(CommandProcessor const& rhs) = delete;
	CommandProcessor(CommandProcessor&& rhs) = delete;
	CommandProcessor& operator=(CommandProcessor const& rhs) = delete;
	CommandProcessor& operator=(CommandProcessor&& rhs) = delete;
	~CommandProcessor() = default;

};  // class CommandProcessor

}  // namespace swx

#endif  // GUARD_command_processor_hpp_08439188501070807
