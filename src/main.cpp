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
using swx::CommandManager;
using swx::TimeLog;

// TODO Interaction between CommandManager,
// CommandProcessor and derived of the latter, seems
// messy, especially in regards to error handling.

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cerr << "Subcommand not provided." << endl;
		cerr << CommandManager::directions_to_get_help() << endl;
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
	CommandManager manager(time_log);
	manager.process_command(argv[1], args);
	return 0;
}
