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

#include "reporting_command.hpp"
#include "activity_filter.hpp"
#include "command.hpp"
#include "config.hpp"
#include "help_line.hpp"
#include "list_report_writer.hpp"
#include "placeholder.hpp"
#include "stream_utilities.hpp"
#include "string_utilities.hpp"
#include "summary_report_writer.hpp"
#include "time_log.hpp"
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::endl;
using std::ostream;
using std::ostringstream;
using std::string;
using std::unique_ptr;
using std::vector;

namespace swx
{

namespace
{
    auto const k_exclude_subactivities_alias = "x";
    auto const k_use_regex_alias = "r";

}  // end anonymous namespace

ReportingCommand::ReportingCommand
(   string const& p_command_word,
    vector<string> const& p_aliases,
    string const& p_usage_summary,
    vector<HelpLine> const& p_help_line,
    TimeLog& p_time_log
):
    Command(p_command_word, p_aliases, p_usage_summary, p_help_line),
    m_time_log(p_time_log)
{
    ostringstream exclude_subactivities_message_stream;
    exclude_subactivities_message_stream
        << "Output only the exact ACTIVITY given; do not include its subactivities "
        << "(ignored if used prior to -" << k_use_regex_alias << ')';
    add_option
    (   vector<string>{k_exclude_subactivities_alias, "exact"},
        exclude_subactivities_message_stream.str(),
        [this]() { m_activity_filter_type = ActivityFilter::Type::exact; }
    );

    ostringstream use_regex_message_stream;
    use_regex_message_stream
        << "Treat ACTIVITY as a regular expression, and include "
        << "all activities that match it (ignored if used prior to -"
        << k_exclude_subactivities_alias << ')';
    add_option
    (   vector<string>{k_use_regex_alias, "regex"},
        use_regex_message_stream.str(),
        [this]() { m_activity_filter_type = ActivityFilter::Type::regex; }
    );

    add_option
    (   vector<string>{"l", "list"},
        "Instead of printing a summary, print a date-ordered list of individual "
            "activity stints during the relevant period",
        [this]() { m_report_flags |= ReportWriter::Flags::show_stints; }
    );

    add_option
    (   vector<string>{"b", "beginning"},
        "In addition to any other information, output the earliest time at "
            "which each activity was conducted during the relevant period (ignored "
            "in list mode)",
        [this]() { m_report_flags |= ReportWriter::Flags::include_beginning; }
    );

    add_option
    (   vector<string>{"e", "ending"},
        "Output in a column to the right of any other info, the latest time at "
            "which each activity was conducted during the relevant period (ignored "
            "in list mode)",
        [this]() { m_report_flags |= ReportWriter::Flags::include_ending; }
    );

    add_option
    (   vector<string>{"csv"},
        "Output in CSV format",
        [this]() { m_report_flags |= ReportWriter::Flags::csv; }
    );

    add_option
    (   vector<string>{"v", "verbose"},
        "Instead of printing the summary in \"tree\" form, print the full name of "\
            "each activity (ignored in list mode or succinct mode)",
        [this]() { m_report_flags |= ReportWriter::Flags::verbose; }
    );

    add_option
    (   vector<string>{"s", "succinct"},
        "Succinct output: show grand total only (ignored in list mode)",
        [this]() { m_report_flags |= ReportWriter::Flags::succinct; }
    );
}

ReportingCommand::~ReportingCommand() = default;

Command::ErrorMessages
ReportingCommand::print_report
(   ostream& p_os,
    Config const& p_config,
    vector<string> const& p_activity_components,
    TimePoint const* p_begin,
    TimePoint const* p_end
)
{
    string comparitor;

    if (p_activity_components.empty())
    {
        m_activity_filter_type = ActivityFilter::Type::always_true;
    }
    else
    {
        auto const expanded = expand_placeholders(p_activity_components, m_time_log);
        comparitor = squish(expanded.begin(), expanded.end());
    }

    unique_ptr<ActivityFilter>
        filter(ActivityFilter::create(comparitor, m_activity_filter_type));
    auto const stints = m_time_log.get_stints(*filter, p_begin, p_end);

    ReportWriter::Options const options
    (   p_config.output_rounding_numerator(),
        p_config.output_rounding_denominator(),
        p_config.output_precision(),
        p_config.output_width(),
        p_config.formatted_buf_len(),
        p_config.time_format()
    );

    unique_ptr<ReportWriter>
        report_writer(ReportWriter::create(stints, options, m_report_flags));
    report_writer->write(p_os);

    return ErrorMessages{};
}

bool
ReportingCommand::does_support_placeholders() const
{
    return true;
}

}  // namespace swx
