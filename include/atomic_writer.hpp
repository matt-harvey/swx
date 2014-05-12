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

#ifndef GUARD_atomic_writer_hpp_7454350268214173
#define GUARD_atomic_writer_hpp_7454350268214173

#include <cstdio>
#include <ios>
#include <string>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace swx
{

/**
 * Clients must specialize this for T to use AtomicWriter for
 * their record type.
 */
template <typename T>
void write_record(FILE& p_outfile, T const& p_record);

/**
 * Supports atomic write operations to plain text files. Append one or
 * more records of type \e T, then call \e commit() to write those
 * records atomically to \e p_filepath.
 *
 * @todo Deploy this for use with both Config and TimeLog.
 *
 * @todo We could move the swapfile creation process into the
 * constructor, hold an open FILE* to the swapfile, and the append
 * operation could simply call write_record directly.
 */
template <typename T>
class AtomicWriter
{
// special member functions
public:
	explicit AtomicWriter
	(	std::string const& p_filepath,
		char p_record_separator = '\n'
	);
	AtomicWriter(AtomicWriter const& rhs) = delete;
	AtomicWriter(AtomicWriter&& rhs) = delete;
	AtomicWriter& operator=(AtomicWriter const& rhs) = delete;
	AtomicWriter& operator=(AtomicWriter&& rhs) = delete;
	~AtomicWriter();

// ordinary member functions
public:
	void append(T const& p_record);
	void cancel();
	void commit();

// member variables
private:
	char const m_record_separator;
	std::string const m_filepath;
	std::vector<T> m_records;

};  // class AtomicWriter

// MEMBER FUNCTION IMPLEMENTATIONS

template <typename T>
AtomicWriter<T>::~AtomicWriter()
{
	cancel();  // TODO Do we want this behaviour?
}

template <typename T>
AtomicWriter<T>::AtomicWriter
(	std::string const& p_filepath,
	char p_record_separator
):
	m_record_separator(p_record_separator),
	m_filepath(p_filepath)
{
}

template <typename T>
void
AtomicWriter<T>::append(T const& p_record)
{
	m_records.push_back(p_record);
	return;
}

template <typename T>
void
AtomicWriter<T>::cancel()
{
	m_records.clear();
	return;
}

template <typename T>
void
AtomicWriter<T>::commit()
{
	char [] swap_filepath = "tmp/file_XXXXXXXX";
	FILE* tempfile = mkstemp(swap_filepath);  // non-portable, but safest
	if (tempfile == -1) throw std::runtime_error("Error opening swap file.");
	FILE* infile = std::fopen(m_filepath.c_str(), "r");
	if (infile == nullptr) throw std::runtime_error("Error opening file to read.");
	std::size_t const buf_size = 4096;
	char buf[buf_size];
	for (std::size_t sz; ; sz = std::fread(buf, 1, buf_size, infile))
	{
		if (std::feof(infile)) break;
		if (sz != buf_size) throw std::runtime_error("Error reading from file.");
	}
	for (auto const& record: m_records) write_record(*outfile, m_record);
	if (std::rename(swap_filepath, m_filepath.c_str()) != 0)
	{
		throw std::runtime_error("Error renaming swap file.");
	}
	m_records.clear();
	return;
}

}  // namespace swx

#endif  // GUARD_atomic_writer_hpp_7454350268214173
