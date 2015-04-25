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

#include "info.hpp"
#include "stream_utilities.hpp"
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>

using std::getenv;
using std::runtime_error;
using std::string;
using std::ostringstream;

namespace swx
{

string
Info::application_name()
{
    return SWX_APPLICATION_NAME;
}

string
Info::version()
{
#   ifdef SWX_RELEASE
        ostringstream oss;
        enable_exceptions(oss);
        oss << SWX_VERSION_MAJOR << '.'
            << SWX_VERSION_MINOR << '.'
            << SWX_VERSION_PATCH;
        return oss.str();
#   else
        return "not applicable";
#   endif
}

string
Info::home_dir()
{
    auto const home_dir = getenv("HOME");  // non-portable
    if (home_dir == nullptr)
    {
        throw runtime_error("Cannot find home directory");
    }
    return string(home_dir);
}

}  // namespace swx
