/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_stream_flag_guard_hpp_42134455890457356
#define GUARD_stream_flag_guard_hpp_42134455890457356

#include <ios>
#include <ostream>

namespace swx
{

class StreamFlagGuard
{
public:
	StreamFlagGuard(std::ostream& p_os);
	StreamFlagGuard(StreamFlagGuard const& rhs) = delete;
	StreamFlagGuard(StreamFlagGuard&& rhs) = delete;
	StreamFlagGuard& operator=(StreamFlagGuard const& rhs) = delete;
	StreamFlagGuard& operator=(StreamFlagGuard&& rhs) = delete;
	~StreamFlagGuard();
	void reset();

private:
	std::ostream& m_os;
	std::ios_base::fmtflags const m_original_flags;

};  // class StreamFlagGuard

}  // namespace swx

#endif  // GUARD_stream_flag_guard_hpp_42134455890457356
