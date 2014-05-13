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
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <stdexcept>
#include <string>

using std::feof;
using std::fopen;
using std::fread;
using std::rename;
using std::runtime_error;
using std::size_t;
using std::string;

namespace swx
{

// TODO Make a simple class to handle FILE* closing via RAII.

AtomicWriter::AtomicWriter(string const& p_filepath):
	m_swapfile(nullptr),
	m_orig_filepath(p_filepath)
{
	// On construction, we immediately copy the orig file to a newly created
	// swap file.

	// NOTE There's a bunch of non-portable stuff in here. POSIX is assumed.
	// This is done consciously: there is no intention of supporting this
	// application for non-POSIX platforms.
	char swap_filepath[] = "tmp/file_XXXXXXXX";
	auto const swapfile_descriptor = mkstemp(swap_filepath);
	if (swapfile_descriptor == -1)
	{
		throw runtime_error("Error opening swap file.");
	}
	m_swap_filepath = swap_filepath;
	m_swapfile = fdopen(swapfile_descriptor, "w+");  // TODO Ensure compatible with mode of file descriptor (including exclusivity thing).
	if (!m_swapfile)
	{
		throw runtime_error("Error opening stream to swap file.");
	}
	FILE* infile = fopen(m_orig_filepath.c_str(), "r");
	if (!infile)
	{
		throw runtime_error("Error opening file to read.");
	}
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
			fclose(infile);  // TODO Respond appropriately if not successfully closed.
			throw runtime_error("Error reading from file.");
		}
		if (fwrite (buf, 1, sz, m_swapfile) != sz)
		{
			fclose(infile);  // TODO Respond appropriately if not successfully closed.
			throw runtime_error("Error writing to file.");
		}
	}
	fclose(infile);  // TODO Respond appropriately if not successfully closed.
	return;
}

AtomicWriter::~AtomicWriter()
{
	// TODO Somehow respond if there was an error closing the swap file;
	// but don't throw from destructor.
	fclose(m_swapfile);
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
