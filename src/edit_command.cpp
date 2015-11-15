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

#include "edit_command.hpp"
#include "command.hpp"
#include "config.hpp"
#include "help_line.hpp"
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using std::endl;
using std::ostream;
using std::string;
using std::system;
using std::vector;

namespace swx
{

EditCommand::EditCommand
(   string const& p_command_word,
    vector<string> const& p_aliases
):
    Command
    (   p_command_word,
        p_aliases,
        "Open the activity log or configuration file in a text editor",
        vector<HelpLine>
        {   HelpLine
            (   "Open the activity log in a text editor; the editor used is "
                    "determined by the \"editor\" configuration setting"
            )
        },
        false
    )
{
    add_option
    (   vector<string>{"config"},
        "Instead of opening the activity log, open the configuration file",
        [this]() { m_open_config_file = true; }
    );
}

EditCommand::~EditCommand() = default;

Command::ErrorMessages
EditCommand::do_process
(   Config const& p_config,
    vector<string> const& p_ordinary_args,
    ostream& p_ordinary_ostream
)
{
    (void)p_ordinary_ostream; (void)p_ordinary_args;  // suppress compiler warning re. unused param.
    string const filepath =
    (   m_open_config_file?
        p_config.filepath():
        p_config.path_to_log()
    );
    string const editor_invokation = p_config.editor() + " " + filepath;
    system(editor_invokation.c_str());
    return ErrorMessages();
}

}  // namespace swx
