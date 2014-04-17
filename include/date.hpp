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

#ifndef GUARD_date_hpp_3886306613630758
#define GUARD_date_hpp_3886306613630758

namespace swx
{

class Date
{
public:
	Date() = default;
	Date(Date const& rhs) = default;
	Date(Date&& rhs) = default;
	Date& operator=(Date const& rhs) = default;
	Date& operator=(Date&& rhs) = default;
	~Date() = default;

};  // class Date

}  // namespace swx

#endif  // GUARD_date_hpp_3886306613630758
