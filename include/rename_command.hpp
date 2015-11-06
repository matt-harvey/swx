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

#ifndef GUARD_rename_command_hpp_21287423428747534
#define GUARD_rename_command_hpp_21287423428747534

#include "activity_filter.hpp"
#include "recording_command.hpp"
#include "time_log.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace swx
{

// begin forward declarations

class Config;

// end forward declarations

class RenameCommand: public RecordingCommand
{
// special member functions
public:
    RenameCommand
    (   std::string const& p_command_word,
        std::vector<std::string> const& p_aliases,
        TimeLog& p_time_log
    );
    RenameCommand(RenameCommand const& rhs) = delete;
    RenameCommand(RenameCommand&& rhs) = delete;
    RenameCommand& operator=(RenameCommand const& rhs) = delete;
    RenameCommand& operator=(RenameCommand&& rhs) = delete;
    virtual ~RenameCommand();

// inherited virtual functions
private:
    virtual ErrorMessages do_process
    (   Config const& p_config,
        std::vector<std::string> const& p_ordinary_args,
        std::ostream& p_ordinary_ostream
    ) override;

    virtual bool does_support_placeholders() const override;

// member variables
private:
    ActivityFilter::Type m_activity_filter_type = ActivityFilter::Type::ordinary;

};  // class RenameCommand

}  // namespace swx

#endif  // GUARD_rename_command_hpp_21287423428747534
