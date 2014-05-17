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

#ifndef GUARD_file_utilties_hpp_21582711730889376
#define GUARD_file_utilties_hpp_21582711730889376

#include <string>

namespace swx
{

/**
 * Checks whether file exists without opening it.
 */
bool file_exists_at(std::string const& p_filepath);

}  // namespace swx

#endif  // GUARD_file_utilties_hpp_21582711730889376
