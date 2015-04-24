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

#include "stream_flag_guard.hpp"
#include <ios>
#include <ostream>

using std::ostream;

namespace swx
{

StreamFlagGuard::StreamFlagGuard(ostream& p_os):
    m_os(p_os),
    m_original_flags(p_os.flags())
{
}

StreamFlagGuard::~StreamFlagGuard()
{
    reset();
}

void
StreamFlagGuard::reset()
{
    m_os.flags(m_original_flags);        
    return;
}

}  // namespace swx
