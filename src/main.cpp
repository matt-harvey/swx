// Copyright 2014 Matthew Harvey

#include "command_router.hpp"
#include "time_conversion.hpp"
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

// TODO Interaction between CommandRouter,
// CommandProcessor and derived of the latter, is
// too messy, especially in regards to error handling.

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		// TODO Make this better.
		cerr << "Subcommand not provided." << endl;
		return 1;
	}
	assert (argc >= 2);
	vector<string> args;
	for (int i = 1; i != argc; ++i)
	{
		string const arg(argv[i]);
		args.push_back(arg);
	}
	CommandRouter router;
	router.process_command(argv[1], args);
	return 0;
}
