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
 * more strings, then call \e commit() to write those strings atomically to
 * \e p_filepath.
 */
class AtomicWriter
{
// special member functions
public:
	explicit AtomicWriter(std::string const& p_filepath);
	AtomicWriter(AtomicWriter const& rhs) = delete;
	AtomicWriter(AtomicWriter&& rhs) = delete;
	AtomicWriter& operator=(AtomicWriter const& rhs) = delete;
	AtomicWriter& operator=(AtomicWriter&& rhs) = delete;
	~AtomicWriter();

// ordinary member functions
public:
	void append(std::string const& p_str);
	void append_line(std::string const& p_str);
	void append_line();
	void commit();

// member variables
private:
	std::FILE* m_tempfile;
	std::string const m_orig_filepath;
	std::string m_temp_filepath;

};  // class AtomicWriter

}  // namespace swx

#endif  // GUARD_atomic_writer_hpp_7454350268214173
