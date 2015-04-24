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

#ifndef GUARD_recording_command_hpp_9906369540796788
#define GUARD_recording_command_hpp_9906369540796788

#include "command.hpp"
#include "help_line.hpp"
#include "time_log.hpp"
#include <string>
#include <vector>

namespace swx
{

class RecordingCommand: public Command
{
// special member functions
public:
    RecordingCommand
    (   std::string const& p_command_word,
        std::vector<std::string> const& p_aliases,
        std::string const& p_usage_summary,
        std::vector<HelpLine> const& p_help_lines,
        bool p_accept_ordinary_args,
        TimeLog& p_time_log
    );
    RecordingCommand(RecordingCommand const& rhs) = delete;
    RecordingCommand(RecordingCommand&& rhs) = delete;
    RecordingCommand& operator=(RecordingCommand const& rhs) = delete;
    RecordingCommand& operator=(RecordingCommand&& rhs) = delete;
    virtual ~RecordingCommand();

// ordinary member functions
protected:
    TimeLog& time_log();

// inherited virtual functions
protected:
    virtual std::string do_get_category() const override;

// member variables
private:
    TimeLog& m_time_log;    

};  // class RecordingCommand

}  // namespace swx

#endif  // GUARD_recording_command_hpp_9906369540796788
