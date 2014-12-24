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

#ifndef GUARD_arithmetic_hpp_5860378041798425
#define GUARD_arithmetic_hpp_5860378041798425

namespace swx
{

/**
 * @returns \e true if it is "safe" to add \e p_x and \e p_y, i.e. it won't
 * cause wraparound or overflow; otherwise, returns \e false.
 *
 * This function template is currently only instantiated for the type
 * <em>unsigned long long</em>. Passing other types will cause compilation
 * failure.
 */
template <typename T>
bool addition_is_safe(T p_x, T p_y);

/**
 * rounds \e x to the nearest multiple of 1.0 * num / den.
 */
double round(double x, int num, int den);

}  // namespace swx

#endif  // GUARD_arithmetic_hpp_5860378041798425
