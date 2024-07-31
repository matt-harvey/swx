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

#include "config_command.hpp"
#include "command.hpp"
#include "config.hpp"
#include "help_line.hpp"
#include <cstdlib>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::endl;
using std::ostream;
using std::runtime_error;
using std::string;
using std::system;
using std::vector;

namespace swx
{

ConfigCommand::ConfigCommand
(   string const& p_command_word,
    vector<string> const& p_aliases
):
    Command
    (   p_command_word,
        p_aliases,
        "Print configuration information",
        vector<HelpLine>
        {   HelpLine("Print summary of all configuration settings")
        },
        false
    )
{
    add_option
    (   vector<string>{"e", "edit"},
        "Instead of printing configuration settings, open the configuration "
            "file using the text editor determined by the \"editor\" "
            "configuration setting",
        [this]() { m_editing_option = true; }
    );
    add_option
    (   vector<string>{"filepath"},
        "Instead of printing configuration settings, print the location of the "
            "configuration file",
        [this]() { m_filepath_option = true; }
    );
}

ConfigCommand::~ConfigCommand() = default;

/** @throws std::runtime_error if there's an error opening the text editor */
Command::ErrorMessages
ConfigCommand::do_process
(   Config const& p_config,
    vector<string> const& p_ordinary_args,
    ostream& p_ordinary_ostream
)
{
    (void)p_ordinary_args;  // silence compiler warning re. unused param.
    if (m_editing_option)
    {
        string const editor_invocation = p_config.editor() + " " + p_config.filepath();
        auto const editor_result = system(editor_invocation.c_str());
        if (editor_result != EXIT_SUCCESS) {
            throw runtime_error(
                "Could not open text editor with command '" + editor_invocation + "'. " +
                "Check your 'editor' setting in " + p_config.filepath() + "."
            );
        }
    }
    else if (m_filepath_option)
    {
        p_ordinary_ostream << p_config.filepath() << endl;
    }
    else
    {
        p_ordinary_ostream << p_config.summary();
    }
    return {};
}

}  // namespace swx
