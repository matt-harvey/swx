/*
 * Copyright 2014 Matthew Harvey
 */

#include "command_processor.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace swx
{

CommandProcessor::CommandProcessor()
{
}

CommandProcessor::~CommandProcessor()
{
}

void
CommandProcessor::process(vector<string> const& p_args)
{
	do_process(p_args);
	return;
}

}  // namespace swx
