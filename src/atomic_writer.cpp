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

#include "atomic_writer.hpp"
#include "info.hpp"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

using std::feof;
using std::fopen;
using std::fread;
using std::rename;
using std::runtime_error;
using std::size_t;
using std::string;
using std::vector;

namespace swx
{

namespace
{
	class FileStreamGuard
	{
	public:
		explicit FileStreamGuard(FILE* p_file_stream):
			m_file_stream(p_file_stream)
		{
		}
		~FileStreamGuard()
		{
			if (m_file_stream)
			{
				// TODO Respond appropriately if not successfully closed.
				fclose(m_file_stream);
			}
		}
	private:
		FILE* m_file_stream;
	};
}

// TODO Make a simple class to handle FILE* closing via RAII.

AtomicWriter::AtomicWriter(string const& p_filepath):
	m_swapfile(nullptr),
	m_orig_filepath(p_filepath)
{
	// TODO Need to ensure umask is set appropriately. See docs for
	// mkstemp.

	// On construction, we immediately copy the orig file to a newly created
	// swap file.

	// NOTE There's a bunch of non-portable stuff in here. POSIX is assumed.
	// This is done consciously: there is no intention of supporting this
	// application for non-POSIX platforms.
	string const sf_template_str = Info::home_dir() + "/.swx_swap_XXXXXX";
	vector<char> vec(sf_template_str.begin(), sf_template_str.end());
	vec.push_back(0);
	char* const swap_filepath = &vec[0];
	int const swapfile_descriptor = mkstemp(swap_filepath);
	if (swapfile_descriptor == -1)
	{
		throw runtime_error("Error opening swap file.");
	}
	m_swap_filepath = swap_filepath;
	m_swapfile = fdopen(swapfile_descriptor, "w+");  
	if (!m_swapfile)
	{
		throw runtime_error("Error opening stream to swap file.");
	}
	FILE* infile = fopen(m_orig_filepath.c_str(), "r");
	if (!infile)
	{
		throw runtime_error("Error opening file to read.");
	}
	FileStreamGuard infile_guard(infile);
	size_t const buf_size = 4096;
	char buf[buf_size];
	size_t sz;
	bool reached_end = false;
	while (!reached_end)
	{
		sz = fread(buf, 1, buf_size, infile);
		if (feof(infile))
		{
			reached_end = true;
		}
		if ((sz != buf_size) && !reached_end)
		{
			throw runtime_error("Error reading from file.");
		}
		if (fwrite (buf, 1, sz, m_swapfile) != sz)
		{
			throw runtime_error("Error writing to file.");
		}
	}
	return;
}

AtomicWriter::~AtomicWriter()
{
	// TODO Somehow respond if there was an error closing and deleting the swap
	// file; but don't throw from destructor.
	if (m_swapfile)
	{
		fclose(m_swapfile);
	}
	
	// Might fail if m_swapfile wasn't created - or for other reasons.
	remove(m_swap_filepath.c_str());
}

void
AtomicWriter::append(string const& p_str)
{
	auto const result = fprintf(m_swapfile, p_str.c_str());
	if (result != p_str.size())
	{
		throw runtime_error("Error appending to file.");	
	}
	return;
}

void
AtomicWriter::commit()
{
	if (rename(m_swap_filepath.c_str(), m_orig_filepath.c_str()))
	{
		throw runtime_error("Error renaming swap file.");
	}
	return;
}

}  // namespace swx
