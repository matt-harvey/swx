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
 * Supports atomic write operations to plain text files. Append one or
 * more records of type \e T, then call \e commit() to write those
 * records atomically to \e p_filepath.
 *
 * @todo Deploy this for use with both Config and TimeLog.
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
	// NOTE tmpname has race condition - but see below.
	// Could use POSIX function mkstemp, but that gives us a FILE*, so
	// would need to rework the below accordingly.
	std::string const swap_filepath(std::tmpnam(nullptr));

	// Clobber on open here will thwart any malicious exploiter of race
	// condition - and frustrate any unlucky receiver of the same
	// filename. The worst that can happen to us is that we can't open
	// the file because it's already open.
	std::ofstream out(swap_filepath);

	// TODO Ensure this throws if out already has error condition.
	out.exceptions(std::ios::failbit | std::ios::badbit);

	// TODO MEDIUM PRIORITY Would be much faster if in and out streams were
	// created in std::ios::binary mode. But then we want to keep the stream
	// open (don't want to close for fear of race condition), to write our
	// records to it in non-binary mode. Is there a solution to this?
	ifstream in(p_source_filepath.c_str());
	in.exceptions(ios::badbit | ios::failbit);
	out << in.rdbuf();  // TODO Does this work if non-binary?

	std::copy
	(	m_records.begin(),
		m_records.end(),
		std::ostream_iterator<T>(out, m_record_separator)
	);
	// TODO Could another process modify the contents of the swap file
	// before we do the rename?

	// TODO Do we need newline at end?
	if (std::rename(swap_filepath.c_str(), m_filepath.c_str()) != 0)
	{
		// TODO Could use std::strerror to get more specific error
		// message. Should probably also put name of both swap file and
		// destination file into message.
		throw std::runtime_error("Error renaming swap file.");
	}
	m_records.clear();
	return;
}

}  // namespace swx

#endif  // GUARD_atomic_writer_hpp_7454350268214173
