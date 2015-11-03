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
#include "command.hpp"
#include "config.hpp"
#include "csv_list_report_writer.hpp"
#include "csv_summary_report_writer.hpp"
#include "help_line.hpp"
#include "human_list_report_writer.hpp"
#include "human_summary_report_writer.hpp"
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
    auto const k_exclude_subactivities_flag = 'x';
    auto const k_use_regex_flag = 'r';

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
        << "(cannot be used in combination with -" << k_use_regex_flag << ')';
    add_option
    (   k_exclude_subactivities_flag,
        exclude_subactivities_message_stream.str(),
        &m_exclude_subactivities
    );

    ostringstream use_regex_message_stream;
    use_regex_message_stream
        << "Treat ACTIVITY as a (POSIX extended) regular expression, and include "
        << "all activities that match it (cannot be used in combination with -"
        << k_exclude_subactivities_flag << ')';
    add_option
    (   k_use_regex_flag,
        use_regex_message_stream.str(),
        &m_use_regex
    );

    add_option
    (   'l',
        "Instead of printing a summary, print a date-ordered list of individual "
            "activity stints during the relevant period",
        &m_show_stints
    );

    add_option
    (   'b',
        "In addition to any other information, output the earliest time at "
            "which each activity was conducted during the relevant period (ignored "
            "in list mode)",
        &m_show_beginning
    );

    add_option
    (   'e',
        "Output in a column to the right of any other info, the latest time at "
            "which each activity was conducted during the relevant period (ignored "
            "in list mode)",
        &m_show_end
    );

    add_option
    (   'c',
        "Output in CSV format",
        &m_produce_csv
    );

    add_option
    (   'v',
        "Instead of printing the summary in \"tree\" form, print the full name of "\
            "each activity (ignored in list mode or succinct mode)",
        &m_be_verbose
    );

    add_option
    (   's',
        "Succinct output: show grand total only (ignored in list mode)",
        &m_be_succinct
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
    unique_ptr<string> activity_ptr;
    if (m_use_regex && m_exclude_subactivities)
    {
        ostringstream oss;
        enable_exceptions(oss);
        oss << '-' << k_use_regex_flag
            << " cannot be used in combination with "
            << '-' << k_exclude_subactivities_flag << '.';
        return ErrorMessages{ oss.str() };
    }
    if (!p_activity_components.empty())
    {
        auto const expanded = expand_placeholders(p_activity_components, m_time_log);
        activity_ptr.reset(new string(squish(expanded.begin(), expanded.end())));
        if (!m_exclude_subactivities && !m_use_regex)
        {
            *activity_ptr = "^" + *activity_ptr + "($|[[:space:]].*)";
        }
    }
    auto const stints = m_time_log.get_stints
    (   activity_ptr.get(),
        p_begin,
        p_end,
        (m_use_regex || !m_exclude_subactivities)
    );
    ReportWriter::Options const options
    (   p_config.output_rounding_numerator(),
        p_config.output_rounding_denominator(),
        p_config.output_precision(),
        p_config.output_width(),
        p_config.formatted_buf_len(),
        p_config.time_format()
    );
    unique_ptr<ReportWriter> report_writer;
    if (m_show_stints)
    {
        if (m_produce_csv)
        {
            report_writer.reset(new CsvListReportWriter(stints, options));
        }
        else
        {
            report_writer.reset(new HumanListReportWriter(stints, options));
        }
    }
    else
    {
        if (m_produce_csv)
        {
            report_writer.reset
            (   new CsvSummaryReportWriter
                (   stints,
                    options,
                    m_show_beginning,
                    m_show_end,
                    m_be_succinct
                )
            );
        }
        else
        {
            report_writer.reset
            (   new HumanSummaryReportWriter
                (   stints,
                    options,
                    m_show_beginning,
                    m_show_end,
                    m_be_verbose,
                    m_be_succinct
                )
            );
        }
    }
    report_writer->write(p_os);
    return ErrorMessages{};
}

bool
ReportingCommand::does_support_placeholders() const
{
    return true;
}

}  // namespace swx
