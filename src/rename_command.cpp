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

#include "rename_command.hpp"
#include "activity_filter.hpp"
#include "command.hpp"
#include "config.hpp"
#include "exact_activity_filter.hpp"
#include "help_line.hpp"
#include "ordinary_activity_filter.hpp"
#include "placeholder.hpp"
#include "recording_command.hpp"
#include "regex_activity_filter.hpp"
#include "stream_utilities.hpp"
#include "string_utilities.hpp"
#include "time_log.hpp"
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::endl;
using std::ostream;
using std::ostringstream;
using std::unique_ptr;
using std::string;
using std::vector;

namespace swx
{

namespace
{
    auto const k_exclude_subactivities_flag = 'x';
    auto const k_use_regex_flag = 'r';

}  // end anonymous namespace

RenameCommand::RenameCommand
(   string const& p_command_word,
    vector<string> const& p_aliases,
    TimeLog& p_time_log
):
    RecordingCommand
    (   p_command_word,
        p_aliases,
        "Rename activity",
        vector<HelpLine>
        {   HelpLine
            (   "Rename ACTIVITY to NAME, both as an activity in its own right, "
                    "and wherever it occurs as an ancestor of other activities",
                "<ACTIVITY> <NAME>"
            )
        },
        true,
        p_time_log
    )
{
    ostringstream exclude_subactivities_message_stream;
    exclude_subactivities_message_stream
        << "Only rename ACTIVITY where it occurs as an activity in its own right, "
        << "leaving unchanged any occurrences as an ancestor of other activities (ignored "
        << "if followed by -" << k_use_regex_flag << ')';
    add_option
    (   k_exclude_subactivities_flag,
        exclude_subactivities_message_stream.str(),
        [this]() { m_activity_filter_type = ActivityFilter::Type::exact; }
    );
    ostringstream use_regex_message_stream;
    use_regex_message_stream
        << "Treat ACTIVITY as a regular expression, and replace it with "
        << "NAME anywhere it matches any activity (ignored if followed by -"
        << k_exclude_subactivities_flag << ')';
    add_option
    (   k_use_regex_flag,
        use_regex_message_stream.str(),
        [this]() { m_activity_filter_type = ActivityFilter::Type::regex; }
    );
}

RenameCommand::~RenameCommand() = default;

Command::ErrorMessages
RenameCommand::do_process
(   Config const& p_config,
    vector<string> const& p_ordinary_args,
    ostream& p_ordinary_ostream
)
{
    (void)p_config;  // silence compiler re. unused param.
    ErrorMessages error_messages;
    if (p_ordinary_args.size() == 2)
    {
        auto const process_arg = [p_ordinary_args](TimeLog& log, size_t i) {
            auto const expanded = expand_placeholders(split(p_ordinary_args[i]), log);
            return squish(expanded.begin(), expanded.end());
        };
        auto const comparitor = process_arg(time_log(), 0);
        auto const new_name = process_arg(time_log(), 1);

        unique_ptr<ActivityFilter>
            activity_filter(ActivityFilter::create(comparitor, m_activity_filter_type));
        auto const count = time_log().rename_activity(*activity_filter, new_name);
        switch (count)
        {
        case 0:
            p_ordinary_ostream << "No matches found." << endl;
            break;
        case 1:
            p_ordinary_ostream << "1 stint matched and renamed" << endl;
            break;
        default:
            // TODO LOW PRIORITY Should probably provide at least an option to
            // print each individual renaming.
            p_ordinary_ostream << count << " stints matched and renamed." << endl;
            break;
        }
    }
    else
    {
        ostringstream oss;
        enable_exceptions(oss);
        oss << "Wrong number of arguments. Required 2, received "
            << p_ordinary_args.size();
        error_messages.push_back(oss.str());
    }
    return error_messages;
}

bool
RenameCommand::does_support_placeholders() const
{
    return true;
}

}  // namespace swx
