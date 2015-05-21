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

namespace
{
    char const k_regex_flag = 'r';
    char const k_list_flag = 'l';
    char const k_beginning_flag ='b';
    char const k_ending_flag = 'e';
    char const k_verbose_flag = 'v';
    char const k_csv_flag = 'c';
    char const k_tree_view_flag = 't';
    char const k_succinct_view_flag = 's';
}

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
    add_boolean_option
    (   k_regex_flag,
        "Treat ACTIVITY as a (POSIX extended) regular expression, and include "
            "all activities that match it"
    );
    add_boolean_option
    (   k_list_flag,
        "Instead of printing a summary, print a date-ordered list of "
            "individual activity stints during the relevant period"
    );
    add_boolean_option
    (   k_beginning_flag,
        "In addition to any other information, output the earliest time at "
            "which each activity was conducted during the relevant period (does "
            "not apply in list mode)"
    );
    add_boolean_option
    (   k_ending_flag,
        "Output in a column to the right of any other info, the latest time at "
            "which each activity was conducted during the relevant period (does "
            "not apply in list mode)"
    );
    add_boolean_option
    (   k_verbose_flag,
        "Print both a date-ordered list of individual activity stints, and a "
            "summary of the time spent on each activity"
    );
    add_boolean_option
    (   k_csv_flag,
        "Output in CSV format"
    );
    add_boolean_option
    (   k_tree_view_flag,
        "Show hierarchical \"tree\" activity structure (ignored in list mode "
            "and in CSV format)"
    );
    add_boolean_option
    (   k_succinct_view_flag,
        "Succinct output: show grand total only (ignored in list mode and in "
            "tree mode)"
    );
}

ReportingCommand::~ReportingCommand() = default;

ostream&
ReportingCommand::print_report
(   ostream& p_os,
    Config const& p_config,
    Flags const& p_flags,
    vector<string> const& p_activity_components,
    TimePoint const* p_begin,
    TimePoint const* p_end
)
{
    bool const csv = p_flags.count(k_csv_flag);
    bool const use_regex = p_flags.count(k_regex_flag);
    bool const show_beginning = p_flags.count(k_beginning_flag);
    bool const show_end = p_flags.count(k_ending_flag);
    bool const verbose = p_flags.count(k_verbose_flag);
    bool const detail = (verbose || p_flags.count(k_list_flag));
    bool const summary = (verbose || !p_flags.count(k_list_flag));
    bool const show_tree = p_flags.count(k_tree_view_flag);
    bool const succinct = p_flags.count(k_succinct_view_flag);

    unique_ptr<string> activity_ptr;
    if (!p_activity_components.empty())
    {
        auto const expanded =
            expand_placeholders(p_activity_components, m_time_log);
        activity_ptr.reset
        (   new string(squish(expanded.begin(), expanded.end()))
        );
    }
    auto const stints =
        m_time_log.get_stints(activity_ptr.get(), p_begin, p_end, use_regex);
    unique_ptr<ListReportWriter> list_writer;
    unique_ptr<SummaryReportWriter> summary_writer;
    ReportWriter::Options const options
    (   p_config.output_rounding_numerator(),
        p_config.output_rounding_denominator(),
        p_config.output_precision(),
        p_config.output_width(),
        p_config.formatted_buf_len(),
        p_config.time_format()
    );
    if (detail)
    {
        if (csv) list_writer.reset(new CsvListReportWriter(stints, options));
        else     list_writer.reset(new HumanListReportWriter(stints, options));
    }
    if (summary)
    {
        if (csv)
        {
            summary_writer.reset
            (   new CsvSummaryReportWriter
                (   stints,
                    options,
                    show_beginning,
                    show_end,
                    succinct
                )
            );
        }
        else
        {
            summary_writer.reset
            (   new HumanSummaryReportWriter
                (   stints,
                    options,
                    show_beginning,
                    show_end,
                    show_tree,
                    succinct
                )
            );
        }
    }
    if (list_writer) list_writer->write(p_os);
    if (list_writer && summary_writer) p_os << endl;
    if (summary_writer) summary_writer->write(p_os);
    return p_os;    
}

string
ReportingCommand::do_get_category() const
{
    return "Reporting";
}

bool
ReportingCommand::does_support_placeholders() const
{
    return true;
}

}  // namespace swx
