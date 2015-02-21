/*
 * Copyright 2014 Matthew Harvey
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "command_manager.hpp"
#include "config.hpp"
#include "info.hpp"
#include "stream_utilities.hpp"
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
using swx::enable_exceptions;
using swx::Info;
using swx::CommandManager;
using swx::Config;
using swx::TimeLog;

// TODO MEDIUM PRIORITY Some of the stuff that's provided to TimeLog
// constructor is also in Config. And then we provide both TimeLog
// instance and Config instance to CommandManager. Seems messy.

int main(int argc, char** argv)
{
	try
	{
		enable_exceptions(cout);
		if (argc < 2)
		{
			cerr << "Command not provided." << endl;
			cerr << CommandManager::directions_to_get_help() << endl;
			return 1;
		}
		assert (argc >= 2);
		vector<string> args(argv + 2, argv + argc);
		auto const config_path = Info::home_dir() + "/.swxrc";  // non-portable
		Config const config(config_path);
		TimeLog time_log
		(	config.path_to_log(),
			config.time_format(),
			config.formatted_buf_len()
		);
		CommandManager manager(time_log);
		manager.process_command(config, argv[1], args);
	}
	catch (std::runtime_error& e)
	{
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
	catch (...)
	{
		// Ensure stack is fully unwound.
		throw;
	}
	return 0;
}
