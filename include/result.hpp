/*
 * Copyright 2015 Matthew Harvey
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

#ifndef GUARD_result_hpp_5099492992786454
#define GUARD_result_hpp_5099492992786454

#include <cassert>
#include <string>
#include <vector>

namespace swx
{

/**
 * Represents a wrapper around an instance of T where there has possibly
 * been an error in some way in attempting to initialize a valid instance of T,
 * in which case this is indicated by populating error messages that can
 * be retrieved from the result.
 */
template <typename T>
class Result
{
// static factory functions
public:
    static Result make_valid(T const& p_content);
    static Result make_invalid(std::vector<std::string> const& p_errors);
    static Result make_invalid(std::string const& p_error);

// special member functions
private:
    explicit Result(T const& p_content = T());
public:
    Result(Result const& rhs) = default;
    Result(Result&& rhs) = default;
    Result& operator=(Result const& rhs) = default;
    Result& operator=(Result&& rhs) = default;
    ~Result() = default;

// ordinary member functions
public:

    /**
     * The caller is responsible for checking whether errors are
     * empty before calling this. Behaviour is undefined if there
     * are errors.
     */
    T const& get() const;

    std::vector<std::string> const& errors() const;

// member variables
private:
    T m_content;
    std::vector<std::string> m_errors;

};  // class Result

// IMPLEMENTATIONS

template <typename T>
Result<T>
Result<T>::make_valid(T const& p_content)
{
    return Result(p_content);
}

template <typename T>
Result<T>
Result<T>::make_invalid(std::vector<std::string> const& p_errors)
{
    Result ret;
    ret.m_errors = (p_errors.empty() ? std::vector<std::string>{std::string()} : p_errors);
    return ret;
}

template <typename T>
Result<T>
Result<T>::make_invalid(std::string const& p_error)
{
    return make_invalid(std::vector<std::string>{p_error});
}

template <typename T>
Result<T>::Result(T const& p_content): m_content(p_content)
{
}

template <typename T>
T const&
Result<T>::get() const
{
    assert (m_errors.empty());
    return m_content;
}

template <typename T>
std::vector<std::string> const&
Result<T>::errors() const
{
    return m_errors;
}

}  // namespace swx

#endif  // GUARD_result_hpp_5099492992786454
