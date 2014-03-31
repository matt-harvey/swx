// Copyright 2014 Matthew Harvey

#include "command_router.hpp"
#include "time_log.hpp"
#include "time_point.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using swx::CommandRouter;
using swx::TimeLog;

// TODO Interaction between CommandRouter,
// CommandProcessor and derived of the latter, seems
// messy, especially in regards to error handling.

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cerr << "Subcommand not provided." << endl;
		cerr << CommandRouter::directions_to_get_help() << endl;
		return 1;
	}
	assert (argc >= 2);
	vector<string> args;
	for (int i = 2; i != argc; ++i)
	{
		string const arg(argv[i]);
		args.push_back(arg);
	}
	string const path_to_time_log("time_log.swx");  // TODO Change this.
	TimeLog time_log(path_to_time_log);
	CommandRouter router(time_log);
	router.process_command(argv[1], args);
	return 0;
}
