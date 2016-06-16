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

#ifndef GUARD_report_writer_hpp_6461996848910114
#define GUARD_report_writer_hpp_6461996848910114

#include "interval_fwd.hpp"
#include "stint.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace swx
{

class ReportWriter
{
// nested types
public:
    struct Options
    {
        /* Holds various options for use by ReportWriter.
         * Options have the same meaning as for Config class, except
         * for p_depth, which is not in Config class, and which refers
         * to the depth to which nodes will be printed when in tree
         * format.
         *
         * @todo MEDIUM PRIORITY It feels wrong that p_depth is not in Config.
         *
         * @todo MEDIUM PRIORITY Do we really need this when we already have
         * Config?
         */
        Options
        (   unsigned int p_output_rounding_numerator,
            unsigned int p_output_rounding_denominator,
            unsigned int p_output_precision,
            unsigned int p_output_width,
            unsigned int p_formatted_buf_len,
            std::string const& p_time_format,
            unsigned int p_depth
        );
        unsigned int const output_rounding_numerator;
        unsigned int const output_rounding_denominator;
        unsigned int const output_precision;
        unsigned int const output_width;
        unsigned int const formatted_buf_len;
        std::string const time_format;
        unsigned int const depth;
    };

    /**
     * Boolean options for configuring the report.
     */
    struct Flags
    {
        using Type = int;
        static Type constexpr none              = 0;
        static Type constexpr include_beginning = (1 << 0);
        static Type constexpr include_ending    = (1 << 1);
        static Type constexpr verbose           = (1 << 2);
        static Type constexpr succinct          = (1 << 3);
        static Type constexpr csv               = (1 << 4);
        static Type constexpr show_stints       = (1 << 5);
    };

// static factory function
public:
    /**
     * Caller receives ownership of the pointer.
     */
    static ReportWriter* create
    (   std::vector<Stint> const& p_stints,
        Options const& p_options,
        Flags::Type p_flags
    );

// special member functions
public:
    ReportWriter
    (   std::vector<Stint> const& p_stints,
        Options const& p_options
    );
    ReportWriter(ReportWriter const& rhs) = delete;
    ReportWriter(ReportWriter&& rhs) = delete;
    ReportWriter& operator=(ReportWriter const& rhs) = delete;
    ReportWriter& operator=(ReportWriter&& rhs) = delete;
    virtual ~ReportWriter();

// ordinary member functions
public:
    void write(std::ostream& p_os);

protected:

    unsigned int output_precision() const;
    unsigned int output_width() const;
    unsigned int output_rounding_numerator() const;
    unsigned int output_rounding_denominator() const;
    std::string const& time_format() const;
    unsigned int formatted_buf_len() const;
    unsigned int depth() const;

    /**
     * Converts a number of seconds to a double representing a number
     * of hours, rounded according to the rounding behaviour specified
     * in the options passed to the constructor.
     */
    double seconds_to_rounded_hours(unsigned long long p_seconds) const;

    /**
     * @returns duration of \e p_interval rounded according to the rounding
     * behaviour specified in the options passed to the constructor.
     */
    double round_hours(Interval const& p_interval) const;


// virtual member functions
private:
    virtual void do_preprocess_stints
    (   std::ostream& p_os,
        std::vector<Stint> const& p_stints
    );
    
    virtual void do_process_stint(std::ostream& p_os, Stint const& p_stint) = 0;

    virtual void do_postprocess_stints
    (   std::ostream& p_os,
        std::vector<Stint> const& p_stints
    );

// member variables
private:
    Options const m_options;
    std::vector<Stint> const& m_stints;

};  // class ReportWriter

}  // namespace swx

#endif  // GUARD_report_writer_hpp_6461996848910114
