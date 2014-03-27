/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_switch_command_processor_hpp_9219790538409627
#define GUARD_switch_command_processor_hpp_9219790538409627

#include "command_processor.hpp"
#include "time_log.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace swx
{

class SwitchCommandProcessor: public CommandProcessor
{
// special member functions
public:
	explicit SwitchCommandProcessor(TimeLog& p_time_log);
	SwitchCommandProcessor(SwitchCommandProcessor const& rhs) = delete;
	SwitchCommandProcessor(SwitchCommandProcessor&& rhs) = delete;
	SwitchCommandProcessor& operator=(SwitchCommandProcessor const& rhs) = delete;
	SwitchCommandProcessor& operator=(SwitchCommandProcessor&& rhs) = delete;
	virtual ~SwitchCommandProcessor();

// inherited virtual functions
private:
	virtual ErrorMessages do_process
	(	Arguments const& p_args,
		std::ostream& p_ordinary_ostream
	) override;

	std::vector<HelpLine> do_get_help_lines() const override;

// member variables
private:
	TimeLog& m_time_log;

};  // class SwitchCommandProcessor

}  // namespace swx

#endif  // GUARD_switch_command_processor_hpp_9219790538409627
