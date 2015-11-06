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

#ifndef GUARD_summary_report_writer_hpp_7957524563166092
#define GUARD_summary_report_writer_hpp_7957524563166092

#include "activity_stats.hpp"
#include "report_writer.hpp"
#include "stint.hpp"
#include "time_point.hpp"
#include <map>
#include <ostream>
#include <vector>

namespace swx
{

class SummaryReportWriter: public ReportWriter
{
// nested types
public:
    struct Flags
    {
        using Type = int;
        static Type constexpr none                  = 0;
        static Type constexpr include_beginning     = (1 << 0);
        static Type constexpr include_ending        = (1 << 1);
        static Type constexpr verbose               = (1 << 2);
        static Type constexpr succinct              = (1 << 3);
    };  // struct Flags

// special member functions
public:
    SummaryReportWriter
    (   std::vector<Stint> const& p_stints,
        Options const& p_options,
        Flags::Type p_flags
    );
    SummaryReportWriter(SummaryReportWriter const& rhs) = delete;
    SummaryReportWriter(SummaryReportWriter&& rhs) = delete;
    SummaryReportWriter& operator=(SummaryReportWriter const& rhs) = delete;
    SummaryReportWriter& operator=(SummaryReportWriter&& rhs) = delete;
    virtual ~SummaryReportWriter();

// inherited virtual member functions
private:
    virtual void do_preprocess_stints
    (   std::ostream& p_os,
        std::vector<Stint> const& p_stints
    ) override;

    virtual void do_process_stint
    (   std::ostream& p_os,
        Stint const& p_stint
    ) override;

    virtual void do_postprocess_stints
    (   std::ostream& p_os,
        std::vector<Stint> const& p_stints
    ) override;

// other virtual member functions
private:
    virtual void do_write_summary
    (   std::ostream& p_os,
        std::map<std::string, ActivityStats> const& p_activity_stats_map
    ) = 0;

// ordinary member functions
protected:
    bool has_flag(Flags::Type p_flag) const;

// member variables
private:
    Flags::Type const m_flags;
    std::map<std::string, ActivityStats> m_activity_stats_map;

};  // class SummaryReportWriter

}  // namespace swx

#endif  // GUARD_summary_report_writer_hpp_7957524563166092
