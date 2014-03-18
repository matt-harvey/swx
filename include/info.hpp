/*
 * Copyright 2014 Matthew Harvey
 */

#ifndef GUARD_info_hpp_9351167767006516
#define GUARD_info_hpp_9351167767006516

#include <string>

namespace swx
{

/**
 * Manages information about the application as a whole, e.g. version.
 */
class Info
{
// special member functions
public:
	Info() = delete;
	Info(Info const& rhs) = delete;
	Info(Info&& rhs) = delete;
	Info& operator=(Info const& rhs) = delete;
	Info& operator=(Info&& rhs) = delete;
	~Info() = delete;

// static functions
public:
	static std::string application_name();
	static std::string version();

};  // class Info

}  // namespace swx

#endif  // GUARD_info_hpp_9351167767006516
