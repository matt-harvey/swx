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
#include "info.hpp"
#include "stream_utilities.hpp"
#include "time_point.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::runtime_error;
using std::string;
using std::vector;
using swx::enable_exceptions;
using swx::Info;
using swx::CommandManager;

int main(int argc, char** argv)
{
    try
    {
        enable_exceptions(cout);
        if (argc < 2)
        {
            cerr << "Command not provided.\n"
                 << CommandManager::directions_to_get_help() << endl;
            return EXIT_FAILURE;
        }
        assert (argc >= 2);
        vector<string> const args(argv + 2, argv + argc);
        auto const config_path = Info::home_dir() + "/.swxrc";  // non-portable
        CommandManager const manager(config_path);
        return manager.process_command(argv[1], args);
    }
    catch (runtime_error& e)
    {
        cerr << "Error: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        // Ensure stack is fully unwound.
        throw;
    }
}
