/*
 * Copyright 2015 Matthew Harvey
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

#ifndef GUARD_current_command_hpp_9615427562787863
#define GUARD_current_command_hpp_9615427562787863

#include "command.hpp"
#include "config_fwd.hpp"
#include "help_line.hpp"
#include "time_log.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace swx
{

class CurrentCommand: public Command
{
// special member functions
public:
    CurrentCommand
    (   std::string const& p_command_word,
        std::vector<std::string> const& p_aliases,
        TimeLog& p_time_log
    );
    CurrentCommand(CurrentCommand const& rhs) = delete;
    CurrentCommand(CurrentCommand&& rhs) = delete;
    CurrentCommand& operator=(CurrentCommand const& rhs) = delete;
    CurrentCommand& operator=(CurrentCommand&& rhs) = delete;
    virtual ~CurrentCommand();

// inherited virtual functions
private:
    virtual ErrorMessages do_process
    (   Config const& p_config,
        std::vector<std::string> const& p_ordinary_args,
        std::ostream& p_ordinary_ostream
    ) override;

// member variables
private:
    bool m_suppress_newline = false;
    TimeLog& m_time_log;

};  // class CurrentCommand

}  // namespace swx

#endif  // GUARD_current_command_hpp_9615427562787863
