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
#include "string_utilities.hpp"
#include "summary_report_writer.hpp"
#include "time_log.hpp"
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

using std::endl;
using std::ostream;
using std::string;
using std::unique_ptr;
using std::vector;

namespace swx
{

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
    add_option
    (   'r',
        "Treat ACTIVITY as a (POSIX extended) regular expression, and include "
            "all activities that match it",
        &m_use_regex
    );
    add_option
    (   'l',
        "Instead of printing a summary, print a date-ordered list of "
            "individual activity stints during the relevant period",
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

ostream&
ReportingCommand::print_report
(   ostream& p_os,
    Config const& p_config,
    vector<string> const& p_activity_components,
    TimePoint const* p_begin,
    TimePoint const* p_end
)
{
    unique_ptr<string> activity_ptr;
    if (!p_activity_components.empty())
    {
        auto const expanded = expand_placeholders(p_activity_components, m_time_log);
        activity_ptr.reset(new string(squish(expanded.begin(), expanded.end())));
    }
    auto const stints = 
        m_time_log.get_stints(activity_ptr.get(), p_begin, p_end, m_use_regex);
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
    return p_os;    
}

bool
ReportingCommand::does_support_placeholders() const
{
    return true;
}

}  // namespace swx
