/*
 * Copyright 2014, 2015 Matthew Harvey
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

#include "time_log.hpp"
#include "activity_filter.hpp"
#include "atomic_writer.hpp"
#include "file_utilities.hpp"
#include "interval.hpp"
#include "regex_activity_filter.hpp"
#include "stint.hpp"
#include "stream_utilities.hpp"
#include "string_utilities.hpp"
#include "time_point.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::getline;
using std::ifstream;
using std::isspace;
using std::ios;
using std::make_pair;
using std::move;
using std::ofstream;
using std::ostringstream;
using std::pair;
using std::remove;
using std::runtime_error;
using std::size_t;
using std::string;
using std::stringstream;
using std::tm;
using std::upper_bound;
using std::unordered_map;
using std::vector;

namespace chrono = std::chrono;

namespace swx
{

/**
 * Provides implementation for TimeLog.
 */
class TimeLog::Impl
{
// nested types
private:
    class Transaction;
    friend class Transaction;
    struct Entry;     // a single entry in the log, registered in the cache
    using Entries = vector<Entry>;
    using ReferenceCount = Entries::size_type;  // number of entries with a given activity
    using ActivityId = pair<string const, ReferenceCount>*;
    using ActivityRegistry = unordered_map<string, ReferenceCount>;

// special member functions
public:
    Impl
    (   string const& p_filepath,
        string const& p_time_format,
        unsigned int p_formatted_buf_len
    );
    Impl() = delete;
    Impl(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl& operator=(Impl&&) = delete;
    ~Impl();

// ordinary member functions
public:

    // These implement the corresponding public functions of TimeLog.

    void append_entry(string const& p_activity);
    string amend_last(string const& p_activity);
    vector<Stint>::size_type rename_activity
    (   ActivityFilter const& p_activity_filter,
        string const& p_new
    );
    vector<Stint> get_stints
    (   ActivityFilter const& p_activity_filter,
        TimePoint const* p_begin,
        TimePoint const* p_end
    );
    string last_activity_to_match(string const& p_regex);
    vector<string> last_activities(size_t p_num);
    bool is_active_at(TimePoint const& p_time_point);
    bool is_active();
    bool has_activity(string const& p_activity);

private:

    // Implementation details.

    // Overall management of in-memory data structures.
    void clear_cache();
    void mark_cache_as_stale();
    void load();
    void save() const;

    // Record that an entry refers to an activity, or that it has ceased
    // to do so. The activity register contains a reference count for each
    // activity and calling these functions causes this to be updated and
    // for an activity to be deleted from the register when it is no
    // longer referred to.
    ActivityId register_activity_reference(string const& p_activity);
    void deregister_activity_reference(ActivityId p_activity_id);

    string const& activity_at(Entry const& p_entry) const;

    void push_entry(string const& p_activity, TimePoint const& p_time_point);
    void pop_entry();

    // Place a new entry at a specific index in m_entries, but only if it
    // would not result in consecutive identical activities. Return true
    // if and only if entry placed.
    bool put_entry
    (   string const& p_activity,
        TimePoint const& p_time_point,
        Entries::size_type p_index
    );

    // Parse a line provided from the log file, returning a pair of
    // activity name and TimePoint.
    pair<string, TimePoint> parse_line
    (   string const& p_entry_string,
        size_t p_line_number
    ) const;

    // Append an entry to the log file.
    void write_entry
    (   AtomicWriter& p_writer,
        string const& p_activity,
        TimePoint const& p_time_point
    ) const;

    string const& id_to_activity(ActivityId p_activity_id) const;
    Entries::const_iterator find_entry_just_before(TimePoint const& p_time_point);

// member variables
private:
    bool m_loaded = false;
    unsigned int m_formatted_buf_len;
    unsigned int m_expected_time_stamp_length;
    string m_filepath;
    Entries m_entries;
    ActivityRegistry m_activity_registry;
    string const m_time_format;
};

// Represents an entry in the in-memory cache of the time log, corresonding
// to a line in the log file.
struct TimeLog::Impl::Entry
{
    Entry(ActivityId p_activity_id, TimePoint const& p_time_point);
    ActivityId activity_id;
    TimePoint time_point;
};

// Provides RAII mechanism for managing changes to time log as a transaction.
class TimeLog::Impl::Transaction
{
public:
    explicit Transaction(TimeLog::Impl& p_time_log);
    Transaction(Transaction const&) = delete;
    Transaction(Transaction&&) = delete;
    Transaction& operator=(Transaction const&) = delete;
    Transaction& operator=(Transaction&&) = delete;
    ~Transaction();
    void commit();
private:
    void rollback();
    bool m_committed = false;
    TimeLog::Impl& m_time_log_impl;
};

// Implementation of public TimeLog class. Implementation defer to Impl.

TimeLog::TimeLog
(   string const& p_filepath,
    string const& p_time_format,
    unsigned int p_formatted_buf_len
):
    m_impl(new Impl(p_filepath, p_time_format, p_formatted_buf_len))
{
}

TimeLog::~TimeLog() = default;

void
TimeLog::append_entry(string const& p_activity)
{
    return m_impl->append_entry(p_activity);
}

string
TimeLog::amend_last(string const& p_activity)
{
    return m_impl->amend_last(p_activity);
}

vector<Stint>::size_type
TimeLog::rename_activity(ActivityFilter const& p_activity_filter, string const& p_new)
{
    return m_impl->rename_activity(p_activity_filter, p_new);
}

vector<Stint>
TimeLog::get_stints
(   ActivityFilter const& p_activity_filter,
    TimePoint const* p_begin,
    TimePoint const* p_end
)
{
    return m_impl->get_stints(p_activity_filter, p_begin, p_end);
}

string
TimeLog::last_activity_to_match(string const& p_regex)
{
    return m_impl->last_activity_to_match(p_regex);
}

vector<string>
TimeLog::last_activities(size_t p_num)
{
    return m_impl->last_activities(p_num);
}

bool
TimeLog::is_active()
{
    return m_impl->is_active();
}

bool
TimeLog::has_activity(string const& p_activity)
{
    return m_impl->has_activity(p_activity);
}

// Implementation of TimeLog::Impl

TimeLog::Impl::Impl
(   string const& p_filepath,
    string const& p_time_format,
    unsigned int p_formatted_buf_len
):
    m_loaded(false),
    m_formatted_buf_len(p_formatted_buf_len),
    m_expected_time_stamp_length
    (   time_point_to_stamp(now(), p_time_format, p_formatted_buf_len).length()
    ),
    m_filepath(p_filepath),
    m_time_format(p_time_format)
{
    assert (m_entries.empty());
    assert (m_activity_registry.empty());
}

TimeLog::Impl::~Impl() = default;

void
TimeLog::Impl::append_entry(string const& p_activity)
{
    Transaction transaction(*this);
    push_entry(p_activity, now()); 
    transaction.commit();
    return;
}

string
TimeLog::Impl::amend_last(string const& p_activity)
{
    Transaction transaction(*this);
    string last_activity;
    if (!m_entries.empty())
    {
        auto const time_point = m_entries.back().time_point;
        last_activity = activity_at(m_entries.back());
        pop_entry();
        push_entry(p_activity, time_point);
    }
    transaction.commit();
    return last_activity;
}

vector<Stint>::size_type
TimeLog::Impl::rename_activity(ActivityFilter const& p_activity_filter, string const& p_new)
{
    // There is far from the most efficient implementation, but it is fairly straightforward.
    // Note we do it this way using put_entry() to avoid consecutive entries with the same
    // activity.
    Transaction transaction(*this);
    Entries::size_type const num_entries = m_entries.size();
    Entries::size_type num_amended = 0;
    Entries::size_type num_written = 0;
    for (Entries::size_type num_read = 0; num_read != num_entries; ++num_read)
    {
        auto const& old_entry = m_entries[num_read];
        auto const& time_point = old_entry.time_point;
        auto const& old_activity = activity_at(old_entry);
        auto const new_activity = p_activity_filter.replace(old_activity, p_new);
        if (new_activity != old_activity)
        {
            ++num_amended;
        }
        if (put_entry(new_activity, time_point, num_written))
        {
            ++num_written; 
        }
    }
    assert (num_written <= m_entries.size());
    while (m_entries.size() != num_written)
    {
        pop_entry();
    }
    transaction.commit();
    return num_amended;
}

vector<Stint>
TimeLog::Impl::get_stints
(   ActivityFilter const& p_activity_filter,
    TimePoint const* p_begin,
    TimePoint const* p_end
)
{
    load();
    vector<Stint> ret;
    auto const e = m_entries.end();
    auto it = (p_begin ? find_entry_just_before(*p_begin) : m_entries.begin());
    auto const n = now();
    for ( ; (it != e) && (!p_end || (it->time_point < *p_end)); ++it)
    {
        string const& activity = id_to_activity(it->activity_id);
        if (p_activity_filter.matches(activity))
        {
            auto tp = it->time_point;
            if (p_begin && (tp < *p_begin)) tp = *p_begin;
            auto const next_it = it + 1;
            auto const done = (next_it == e);
            auto next_tp = (done ? (n > tp ? n: tp) : next_it->time_point);
            if (p_end && (next_tp > *p_end)) next_tp = *p_end;
            assert (next_tp >= tp);
            assert (!p_begin || (tp >= *p_begin));
            assert (!p_end || (next_tp <= *p_end));
            auto const duration = next_tp - tp;
            auto const seconds = chrono::duration_cast<Seconds>(duration);
            Interval const interval(tp, seconds, done);
            ret.push_back(Stint(activity, interval));
        }
    }
    return ret;
}

string
TimeLog::Impl::last_activity_to_match(string const& p_regex)
{
    load();
    RegexActivityFilter const activity_filter(p_regex);
    auto const empty_activity_id = register_activity_reference("");
    for (auto rit = m_entries.rbegin(); rit != m_entries.rend(); ++rit)  // reverse
    {
        auto const id = rit->activity_id;
        if (id != empty_activity_id)
        {
            auto const& activity = id_to_activity(id);
            if (activity_filter.matches(activity))
            {
                return activity;
            }
        }
    }
    return string();
}

vector<string>
TimeLog::Impl::last_activities(size_t p_num)
{
    load();
    vector<string> ret;
    if (m_entries.empty())
    {
        return ret;
    }
    assert (m_entries.size() >= 1);
    for (auto rit = m_entries.rbegin(); rit != m_entries.rend(); ++rit)  // reverse
    {
        if (ret.size() == p_num)
        {
            break;
        }
        auto const& activity = id_to_activity(rit->activity_id);
        if (!activity.empty() && (ret.empty() || (activity != ret.back())))
        {
            ret.push_back(activity);
        }
    }
    assert (ret.size() <= p_num);
    assert (ret.size() <= m_entries.size());
    return ret;
}

bool
TimeLog::Impl::is_active()
{
    load();
    return !(m_entries.empty() || activity_at(m_entries.back()).empty());
}

bool
TimeLog::Impl::has_activity(string const& p_activity)
{
    load();
    return m_activity_registry.find(p_activity) != m_activity_registry.end();
}

void
TimeLog::Impl::clear_cache()
{
    m_entries.clear();
    m_activity_registry.clear();
    mark_cache_as_stale();
    return;
}

void
TimeLog::Impl::mark_cache_as_stale()
{
    m_loaded = false;
}

void
TimeLog::Impl::load()
{
    if (!m_loaded)
    {
        clear_cache();
        if (file_exists_at(m_filepath))
        {
            ifstream infile(m_filepath.c_str());
            enable_exceptions(infile);
            string line;
            size_t line_number = 1;
            while (infile.peek() != EOF)
            {
                getline(infile, line);
                pair<string, TimePoint> const parsed_line = parse_line(line, line_number);
                auto const& activity = parsed_line.first;
                auto const& time_point = parsed_line.second;
                if (!m_entries.empty() && (time_point < m_entries.back().time_point))
                {
                    ostringstream oss;
                    enable_exceptions(oss);
                    oss << "Time log entries out of order at line " << line_number << '.'; 
                    throw runtime_error(oss.str());
                }
                push_entry(activity, time_point);
                ++line_number;
            }
            if (!m_entries.empty() && (m_entries.back().time_point > now()))
            {
                throw runtime_error
                (   "The final entry in the time log is future-dated. "
                    "Future dated entries are not supported."
                );
            }
        }
        m_loaded = true;
    }
    return;
}

void
TimeLog::Impl::save() const
{
    AtomicWriter writer(m_filepath);
    for (auto const& entry: m_entries)
    {
        write_entry(writer, activity_at(entry), entry.time_point);
    }
    writer.commit();
    return;
}

TimeLog::Impl::ActivityId
TimeLog::Impl::register_activity_reference(string const& p_activity)
{
    auto const it = m_activity_registry.find(p_activity);
    if (it == m_activity_registry.end())
    {
        return &*(m_activity_registry.emplace(p_activity, 1).first);
    }
    ++it->second;
    return &*it;
}

void
TimeLog::Impl::deregister_activity_reference(ActivityId p_activity_id)
{
    assert (p_activity_id->second > 0);
    auto const new_reference_count = --p_activity_id->second; 
    if (new_reference_count == 0)
    {
        // Pointer (p_activity_id) is stable, but iterator is not! We have
        // to erase via the iterator or via key.
        m_activity_registry.erase(p_activity_id->first);
    }
    return;
}

string const&
TimeLog::Impl::activity_at(Entry const& p_entry) const
{
    return id_to_activity(p_entry.activity_id);
}

void
TimeLog::Impl::push_entry(string const& p_activity, TimePoint const& p_time_point)
{
    auto const next_activity_id = register_activity_reference(p_activity);
    if (!m_entries.empty() && (next_activity_id == m_entries.back().activity_id))
    {
        // avoid consecutive entries with the same activity
        deregister_activity_reference(next_activity_id);
    }
    else
    {
        m_entries.emplace_back(next_activity_id, p_time_point);
    }
}

bool
TimeLog::Impl::put_entry
(   string const& p_activity,
    TimePoint const& p_time_point,
    Entries::size_type p_index
)
{
    auto const old_activity_id = m_entries[p_index].activity_id;
    auto const new_activity_id = register_activity_reference(p_activity);

    // prevent consecutive identical activities
    if ((p_index != 0) && (m_entries[p_index - 1].activity_id == new_activity_id))
    {
        deregister_activity_reference(new_activity_id);
        return false;
    }
    deregister_activity_reference(old_activity_id);
    m_entries[p_index] = Entry(new_activity_id, p_time_point);
    return true;
}

void
TimeLog::Impl::pop_entry()
{
    deregister_activity_reference(m_entries.back().activity_id);
    m_entries.pop_back();
}

pair<string, TimePoint>
TimeLog::Impl::parse_line(string const& p_entry_string, size_t p_line_number) const
{
    if (p_entry_string.size() < m_expected_time_stamp_length)
    {
        ostringstream oss;
        enable_exceptions(oss);
        oss << "Error parsing the time log at line " << p_line_number << '.';
        throw runtime_error(oss.str());
    }
    auto it = p_entry_string.begin() + m_expected_time_stamp_length;
    assert (it > p_entry_string.begin());
    string const time_stamp(p_entry_string.begin(), it);
    auto const time_point = time_stamp_to_point(time_stamp, m_time_format);
    auto const activity = trim(string(it, p_entry_string.end()));
    return make_pair(activity, time_point);
}

void
TimeLog::Impl::write_entry
(   AtomicWriter& p_writer,
    string const& p_activity,
    TimePoint const& p_time_point
) const
{
    p_writer.append(time_point_to_stamp(p_time_point, m_time_format, m_formatted_buf_len));
    if (!p_activity.empty())
    {
        p_writer.append(" ");
        p_writer.append(p_activity);
    }
    p_writer.append("\n");
    return;
}

string const&
TimeLog::Impl::id_to_activity(ActivityId p_activity_id) const
{
    return p_activity_id->first;
}

vector<TimeLog::Impl::Entry>::const_iterator
TimeLog::Impl::find_entry_just_before(TimePoint const& p_time_point)
{
    load();
    auto const comp = [](Entry const& lhs, Entry const& rhs)
    {
        return lhs.time_point < rhs.time_point;
    };
    auto const b = m_entries.begin(), e = m_entries.end();
    Entry const dummy(0, p_time_point);
    auto it = upper_bound(b, e, dummy, comp);
    for ( ; (it != b) && ((it == e) || (it->time_point > p_time_point)); --it)
    {
    }
    return it;
}

// Implementation of TimeLog::Impl::Entry

TimeLog::Impl::Entry::Entry(ActivityId p_activity_id, TimePoint const& p_time_point):
    activity_id(p_activity_id),
    time_point(p_time_point)
{
}

// Implementation of TimeLog::Impl::Transaction

TimeLog::Impl::Transaction::Transaction(TimeLog::Impl& p_time_log_impl):
    m_time_log_impl(p_time_log_impl)
{
    m_time_log_impl.load();
}

TimeLog::Impl::Transaction::~Transaction()
{
    if (!m_committed)
    {
        rollback();
    }
}

void
TimeLog::Impl::Transaction::commit()
{
    m_time_log_impl.save();
    m_committed = true;
}

void
TimeLog::Impl::Transaction::rollback()
{
    m_time_log_impl.mark_cache_as_stale();
}

}  // namespace swx
