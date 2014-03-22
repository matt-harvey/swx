/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_activity_command_processor_hpp_23685825347754091
#define GUARD_activity_command_processor_hpp_23685825347754091

#include "command_processor.hpp"
#include "time_log.hpp"

namespace swx
{

/**
 * @todo Implement.
 */
class ActivityCommandProcessor: public CommandProcessor
{
// special member functions
public:
	ActivityCommandProcessor(TimeLog& p_time_log);
	ActivityCommandProcessor(ActivityCommandProcessor const& rhs) = delete;
	ActivityCommandProcessor(ActivityCommandProcessor&& rhs) = delete;
	ActivityCommandProcessor& operator=(ActivityCommandProcessor const& rhs) = delete;
	ActivityCommandProcessor& operator=(ActivityCommandProcessor&& rhs) = delete;
	virtual ~ActivityCommandProcessor();

// inherited virtual functions
private:
	virtual ErrorMessages do_process
	(	Arguments const& p_args,
		std::ostream& p_ordinary_ostream
	) override;

	std::string do_provide_help_string
	(	std::string const& p_command_invocation
	) const override;

private:
	TimeLog& m_time_log;

};  // class ActivityCommandProcessor

}  // namespace swx

#endif  // GUARD_activity_command_processor_hpp_23685825347754091
