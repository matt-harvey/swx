/*
 * Copyright 2014, 2015 Matthew Harvey
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

#ifndef GUARD_command_hpp_08439188501070807
#define GUARD_command_hpp_08439188501070807

#include "exit_code.hpp"
#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace swx
{

// begin forward declarations

class Config;
class HelpLine;

// end forward declarations

/**
 * Represents a command that processes arguments received from
 * the command line.
 *
 * To implement a specific command, inherit from this class and implement
 * its virtual functions.
 */
class Command
{
// nested types
protected:
    using ErrorMessages = std::vector<std::string>;
private:
    struct Impl;

// special member functions
protected:
    Command
    (   std::string const& p_command_word,
        std::vector<std::string> const& p_aliases,
        std::string const& p_usage_summary,
        std::vector<HelpLine> const& p_help_lines,
        bool p_accept_ordinary_args = true
    );
public:
    Command(Command const& rhs) = delete;
    Command(Command&& rhs) = delete;
    Command& operator=(Command const& rhs) = delete;
    Command& operator=(Command&& rhs) = delete;
    virtual ~Command();

protected:
    
    /**
     * @param p_aliases the strings passed to the command line to activate
     * the option. Can be short form (a single character string) or long form
     * (a multiple character string).
     *
     * @param p_help_line HelpLine describing the usage of the option
     *
     * @param p_callback is a callable to be called when the given option is
     * encountered when parsing the args.
     *
     * @param p_arg_target if non-null, signifies that the option requires an
     * argument, the contents of which, if encountered during parsing, will be assigned to
     * <em>*p_arg_target</em>
     */
    void add_option
    (   std::vector<std::string> const& p_aliases,
        HelpLine const& p_help_line,
        std::function<void()> const& p_callback,
        std::string* p_arg_target = nullptr
    );

public:
    ExitCode process
    (   Config const& p_config,
        std::vector<std::string> const& p_args,
        std::ostream& p_ordinary_ostream,
        std::ostream& p_error_ostream
    );

    std::string usage_summary() const;
    std::string usage_descriptor() const;
    std::string const& command_word() const;
    std::vector<std::string> const& aliases() const;

// virtual functions
private:
    virtual ErrorMessages do_process
    (   Config const& p_config,
        std::vector<std::string> const& p_ordinary_args,
        std::ostream& p_ordinary_ostream
    ) = 0;

    /**
     * If you use placeholders in a derived class, override this to
     * return \e true, so that the help string is modified accordingly.
     *
     * TODO MEDIUM PRIORITY This would be better as an instance variable that is
     * initialized in the protected constructor. That could then be passed on to
     * the Impl class.
     */
    virtual bool does_support_placeholders() const;

// member variables
private:
    std::unique_ptr<Impl> m_impl;

};  // class Command

}  // namespace swx

#endif  // GUARD_command_hpp_08439188501070807
