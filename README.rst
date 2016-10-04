Overview
========

``swx`` is a command line application for keeping track of the amount of
time you spend on different activities.

Installation
============

You should be able to install and run ``swx`` on any Unix-like operating system (Linux, OSX, BSD)
that has a reasonably conformant C++ compiler that can build C++11. ``swx`` is not compatible
with Windows.

Installing with Homebrew
------------------------

If you're using the Homebrew package manager, installation is simple::

  brew install matt-harvey/tap/swx

Alternatively, you can do::

  brew tap matt-harvey/tap
  brew install swx

Installing manually
-------------------

If you want to build and install ``swx`` manually from source, first ensure you have `CMake
<http://www.cmake.org/>`_ installed, and that your system has a C++ compiler that supports C++11.
(If you are using GCC, you will need at least version 4.9.) In addition, if you want to run the
tests, you need the Boost unit test framework, available from http://www.boost.org.

Download and unzip the ``swx`` source code, ``cd`` into the project root, and configure the build::

  cmake -D CMAKE_BUILD_TYPE=Release .

Then to build and install, run ``make install``. (Depending on your system, you may need to
prefix this with ``sudo``.)

If you want to build without installing, just run ``make``; or to build and run the test suite, run
``make run_tests``.

Usage
=====

Quick summary
-------------

Start work on a new activity: ``swx switch -c <activity>``, or ``swx s -c <activity>``

Switch to an existing activity: ``swx s <activity>``

Record a switch to an existing activity at a particular time: ``swx s <activity> --at <hh:mm>``

Stop working on any activity: ``swx s``

Resume work on the most recent activity: ``swx resume``

Switch to the most recent activity that matches a regular expression: ``swx s -r <regex>``

Switch to a "child activity" of the current activity: ``swx s <current-activity> <child-activity>``,
or just: ``swx s _ <child-activity>``

Switch to the "parent activity" of the current activity: ``swx s __``

Switch to a "sibling activity" of the current activity: ``swx s __ <sibling-activity>``

Print a summary of today's activities in tree form: ``swx day``, or ``swx d``

Print a time-ordered list of today's individual activity stints: ``swx d -l``

Print yesterday's activities: ``swx d -a1``

Print activities of two days ago: ``swx d -a2``

Print a summary of the entire activity log: ``swx print``, or ``swx p``

Print a summary of activities since a given date and time: ``swx p -f <YYYY-MM-DDThh:mm>``

Print a summary of activitites between two times: ``swx p -f <YYYY-MM-DDThh:mm> -t <YYYY-MM-DDThh:mm>``

Print just the name of the current activity: ``swx current``, or ``swx c``

Print a summary of a given activity and its sub-activities: ``swx p <activity>``

Print a summary of activities matching a regular expression: ``swx p -r <regex>``

Open the time log for editing: ``swx edit``, or ``swx e``

Get configuration info: ``swx config``

Open the configuration file for editing: ``swx config -e``

Get general help: ``swx help``

Get help on a particular command: ``swx help <command>``

General command structure
-------------------------

To use ``swx``, you enter a brief "switching" command each time you start an
activity, end an activity, or switch from one activity to another. ``swx``
makes a timestamped record of each such "transition" in a plain text file—which
you are free to peruse and edit. Then when you want a summary of how you have
spent your time, enter one of the reporting commands—which provide various
filtering and output options—and ``swx`` will analyze the text file and
output the requested information.

Like ``git`` and various other command-line programs, ``swx`` comes with a range
of subcommands. You can see a list of these by entering ``swx help``. The basic
pattern of usage is::

    swx <COMMAND> [OPTIONS...] [ARGUMENTS...] [OPTIONS...]

Options to ``<COMMAND>`` can be entered indifferently either before or after
``[ARGUMENTS...]``, but cannot appear before ``<COMMAND>``.

The "switch" command
--------------------

Suppose you start working on the activity of "answering emails". You would come
up with a name for this activity, say ``answering-emails``. When you first start
working on this activity, you would enter the following at the command line::

    swx switch answering-emails -c

You can use the alias ``s`` if you don't want to type ``switch``::

    swx s answering-emails -c

The ``-c`` option tells the ``switch`` command that this is the first time you
are working on this activity: it will protest if you try to create a new activity
without this option. This guards against error in case you think you're creating
a new activity, but accidentally give it the same name as an existing one. On
subsequent occasions, when you switch back to an already-used activity, you
would omit the ``-c``—and again ``swx`` will helpfully protest in case you
think you're reusing an existing activity, but aren't.

Like all options in ``swx``, the ``-c`` can be entered either before or after
the other arguments.

Suppose you stop answering emails and restart work on a previous activity, say
"spreadsheeting". You record a transition from one activity to another, by
entering ``swx switch`` (or ``swx s``) plus the name of the activity that you
are switching *to*, in this case::

    swx s spreadsheeting

If you cease doing any activity at all (or at least, any activity you care about
recording), you record this cessation by simply entering::

    swx s

If you pass the ``-r`` option to ``swx switch``, then the activity argument
will be treated as a regular expression, rather than an exact activity name.
A switch will then be recorded to the most recently active activity the name
of which matches that regular expression. This can save a fair bit of typing
when switching back to a recently used activity. For example, suppose you are
currently working on "emails customer-service", and the activity before that
was "emails admin", and the one before that was "emails suppliers". Then you
could switch back to "emails suppliers" simply by typing ``swx s -r sup``.
(Note the regular expression grammar that is used is the modified ECMAScript
grammar that is used by default by the C++ standard library.)

If you pass the ``-a`` option to ``swx switch``, then instead of simply
switching to the new activity "from now on", the time log will rather be
amended so that the activity of the current stint is entirely *replaced* with
the activity being switched to. For example, suppose you have worked on
"email" for 0.5 hours followed by "spreadsheeting" for 2 hours. If you enter
``swx s -ac cleaning``, then the time log will be amended so that it now
reflects a sequence of activity consisting of 0.5 hours of "email"
followed by 2 hours of "cleaning". Note the ``-c`` option is also used in this
example because we are creating a new activity. You can just as well use ``swx
switch -a`` to replace the current stint's activity with another activity that
also already exists. Continuing with the current example, if you entered ``swx
s -a email``, the time log would be revised to reflect a single 2.5-hour stint
of "email".

If ``-a`` is used without an argument, then it will effectively erase the
current activity stint, so that it becomes, in effect, a stint of inactivity.

If the ``--at`` option is used with a timestamp, then instead of being recorded
as happening "now", the switch will be recorded as if it had happened at the
corresponding time. The time provided may not be in the future though, and may
not be earlier than the start time of the current activity stint. If used with
the ``-a`` option, the ``--at`` option will cause the start time of the current
activity stint to be amended, in which case the provided time may not be
earlier than the start time of the previous stint. The timestamp can be
either in short or long form. By default, these are the 24-hour time
format (e.g. "14:23") and ISO date-time format (e.g. "2015-02-28T14:23"),
respectively. These formats can be configured, however (see `Configuration`_).
When the short form is used, it is assumed to refer to the corresponding
time on the current day, i.e. the day the command is run.

Note activity names are case-sensitive.

The "resume" command
--------------------

Suppose you are currently "inactive"—on a lunch break, let's say—and then
you return to work and want to resume the most recent activity you were working
on before your break. Enter ``swx resume`` to record a resumption of the
activity you were working on just before the break. This is equivalent to
entering ``swx switch`` together with the name of the most recent activity.

If you are currently "active", then ``swx resume`` will record a switch to
the activity that was active just before the current one. This is useful for
when you are working on one activity, are briefly interrupted by another
activity, and then want to resume work on the original activity.

Like ``swx switch``, ``swx resume`` accepts the ``--at`` option, if you
wish to specify the resumption as occurring at a particular time other
than "now". The specified time must not be in the future, and must not
be earlier than the start time of the current activity stint.

Reporting commands
------------------

To output a summary of the time you have spent on your various activities,
two "reporting commands" are available::

    swx print
    swx day

Enter ``swx help <COMMAND>`` for detailed usage information in regards to each
of these. They follow a similar pattern, and allow you to enter an activity
name, if you want to see only time spent on a given activity (and its
sub-activities), or to omit the activity name, if you want to see time spent on
all activities.

``swx day`` (or ``swx d``) prints a summary of only the current day's
activities, or, if passed the ``-a`` option with an integer argument *n*, the
activities of *n* days ago. For example, ``swx day -a1`` prints a summary of
yesterday's activities.

``swx print`` (or ``swx p``) will by default print a summary of activity that
is not filtered by time at all. With a timestamp passed to the ``-f`` option,
it will show only activity since the given time; with a timestamp passed to the
``-t`` option, only activity up until the given time. Using these options
combined, you can filter for activity between two times.

By default, activities are summarised in "tree" form, showing the hierarchical
structure of activities, sub-activities and so on (see `Complex activities`_
below). If you pass the ``-v`` option to a reporting command, then activities
will instead be displayed in "verbose" form, showing the full name of each
activity, with activities ordered alphabetically by name. If you pass the
``-l`` option to a reporting command, then instead a list of individual
activity stints will be shown, showing the start and end time, and the
duration of each stint in digital format.

When filtering by activity name, the default behaviour is to filter for the
given activity along with its sub-activities. For example, if you have spent 5
hours on an activity called "emails", and 4 hours on an activity called
"emails customer", then the command ``swx print emails`` will print the full
9 hours spent on both these activities. To print only a given activity without
its sub-activities, use the ``-x`` flag. Thus ``swx print -x emails`` would
print only the 5 hours spent on emails and not the 4 hours spent on "emails
customer".

If you pass the ``-r`` option to a reporting command, then the activity string
you enter will be treated as a regular expression, rather than an exact activity
name. Any activities will then be included in the report for which their
activity name matches this regular expression. (Note this is ignored if used
prior to the ``-x`` flag.) Continuing with example above ``swx print -r mail``
would again capture both "emails" and "emails customer".

If you pass the ``-b`` option to a reporting command, then in addition to the
other info, the earliest time at which each activity was conducted during the
period in question will be printed next to each activity. (This does not apply
when outputting in "list" mode.)

If you pass the ``-e`` option, then in addition to, and to the right of,
any other info, the latest time at which each activity was conducted during
the period in question will be printed next to each activity. (This does not
apply when outputting in "list" mode.)

Note that if ``-b`` and ``-e`` options are both provided, the output from
the ``-e`` command is always printed to the right of that from the ``-b``
command, regardless of the order in which the ``-b`` and ``-e`` options are
provided.

If you provide a non-zero positive integer to the ``--depth`` option, then
the activity tree will be printed only to this depth. (This does not apply in
"list", "succinct" or "verbose" mode.)

If you pass the ``--csv`` option to a reporting command, then the results will
be output in CSV format.

If you pass the ``-s`` option, then the results will be output in "succinct"
format, with the total duration shown only, and no activity names shown. This
does not apply in "list" (``-l``) mode.

The amount of time spent on each activity during the relevant period is shown
in terms of digital hours.

By default, the number of hours shown is rounded to the nearest tenth of
an hour (6 minutes). This behaviour can be changed in the Configuration_.

Complex activities
------------------

Activities are often divided conceptually into sub-activities,
sub-sub-activities and so forth. ``swx`` tries to capture this with the
concept of simple and compound activities. A simple activity is specified
using a single word, not containing whitespace, e.g. ``email``.
A compound activity is specified as multiple words separated by whitespace,
e.g. ``email customer-service``.

When passing the name of a compound activity to a ``swx`` command, it can
generally just be passed directly as multiple arguments to the command, without
enclosing it in quotes. ``swx`` will treat it as single, compound activity.
E.g., entering ``swx switch email customer-service`` is exactly equivalent to
entering ``swx switch 'email customer-service'``. The exception to this is the
"rename" command, which takes two activity names as arguments; if either of
these is a "compound" then it must be enclosed in quotes to avoid ambiguity.

Placeholders
------------

When entering a series of whitespace-separated "activity components" at the
command line (e.g. ``email customer-service``), there are certain "placeholders"
that can stand in for one or more such components, and are expanded accordingly
before the command line is properly processed.

- ``_`` expands into the (name of the) current activity. In our example, if
  the current activity were ``email customer-service``, then ``_`` would expand
  into ``email customer-service``.

- ``__`` expands into the "parent" of the current activity. In our current
  example, this would expand into ``email``.

- ``___`` expands into the parent of the parent of the current activity. In our
  current example, since the parent (``email``) has no parent itself, this would
  simply expand into the empty string.

In general, any number of underscores can be entered (with obviously limited
usefulness) to traverse up the "activity tree" by a corresponding number of
"generations".

If there is no currently active activity, then all placeholders will simply
expand into the empty string.

These placeholders can be inserted anywhere among the command-line arguments
where one or more activity "components" are expected, and will be expanded
accordingly. This can save some typing when switching between closely related
activities, or generating a report on the current activity or related
activities. E.g., if we are currently active on "email customer-service
enquiries" and want to record a switch to "email customer-service
complaints", then we can enter simply ``swx s __ complaints``, rather than
having to enter ``swx s email customer-service complaints``.

The "rename" command
--------------------

``swx rename`` can be used to change the name of an activity. By default, this
renames both the given activity in its own right, and this activity as a
component of any sub-activities. For example, suppose we have recorded an
activity called "email" and an activity called "email customer-service". Then
suppose we do::

  swx rename email electronic-mail

This will cause "email" to become "electronic-mail" and "email customer-service"
to become "electronic-mail customer-service". If we *only* wanted to rename
"email" and *not* "email customer-service", we could use the ``-x`` option
to exclude sub-activities when renaming. Alternatively, the ``-r`` option can
be used to replace every occurrence of the first argument, considered as a regular
expression, with the second argument, anywhwere it occurs in any activity name.

If one of the arguments to ``rename`` consists of more than one word, then
it should be enclosed in quotes so that the program call tell which word
goes with which. E.g.::

  swx rename email 'electronic mail'

Note placeholders will still be expanded within each argument, however.

``swx rename`` will not warn you if the new name is the same name as an
existing activity. In this case, the ``rename`` command will essentially
perform a merge, with stints associated with the first activity being
reassigned to the second activity.

Manually editing the time log
-----------------------------

``swx`` stores a log of your activities in a plain text file, which by default
is located in your home directory, and is named ``.swx``.
You are free to edit this file if you want to change the times or activity names
recorded. The command ``swx edit``, or ``swx e``, will cause the log to be
opened in your default text editor.

When editing the log, be sure to preserve the prescribed timestamp format, and
to leave a space between the timestamp and the activity name (if any) on any
given line. (Lines without an activity name record a cessation of activity.)
Also, the time log must be such that the timestamps appear in ascending order
(or at least, non-descending order). Be sure to preserve this order if you edit
the file manually.

You should not enter future-dated entries: the application will raise an error
if it reads a future-dated entry in the log.

Note that if you simply want to edit the activity of the current activity stint,
this can be achieved more directly by using the ``switch`` command with the ``-a``
("amend") option. (See `The "switch" command`_, above.) Or, if you want to change
the name of an existing activity wherever it occurs, this can also be achieved
with ``swx rename``. (See `The "rename" command`_ above.)

Configuration
-------------

Configuration options are stored in your home directory in the file named
``.swxrc``, which will be created the first time you run the program. The
contents of this file should be reasonably self-explanatory.

The command ``swx config`` will output a summary of your configuration settings.
Passing ``-e`` to this command will cause the configuration file to be opened
in your default text editor.

Note that if you change the timestamp format, then this will change the format
of timestamps as read from and written to the data file, *without*
retroactively reformatting the timestamps that are already stored. This will
result in parsing errors, unless you are prepared to reformat manually all your
already-entered timestamps to the new format. Both a short and a long timestamp
format are recognized. The long format is used for storing entries in the time
log and when printing reports. When passing timestamps as options to commands,
either format may be used. The short format is used for specifying a time
without date information.

Help and other commands
-----------------------

Enter ``swx current`` (or ``swx c``) to print just the name of the current
activity. If there is no current activity, this will print a blank line.

Enter ``swx help`` to see a summary of usage, or ``swx help <COMMAND>`` to
see a summary of usage for a particular command.

Enter ``swx version`` to see version information.

Uninstalling
============

If you installed ``swx`` using Homebrew, you can uninstall it by running
``brew uninstall swx``.

If you built and installed ``swx`` manually from source, then a file named
``install_manifest.txt`` would have been created in the source directory
when you ran ``make install``. To uninstall ``swx``, you manually need to
remove each of the files in this list (of which there may well be only one).

In addition, the first time you run ``swx``, it will create a configuration
file called ``.swxrc``, in your home directory. Also, the first time you run
``swx switch`` (or ``swx s``), it will create a data file, in which your
activity log will be stored. Unless you have specified otherwise in your
configuration file, this data file will be stored in your home directory, and
will be named ``.swx``. You may or may not want to remove this file if you
uninstall ``swx``.

Miscellaneous
=============

The name "swx" stands for "stopwatch extended", reflecting that the application
works essentially like a stopwatch which has been extended with various additional
functionality.

Contact
=======

You are welcome to contact me about this project at:

software@matthewharvey.net

Legal
=====

Copyright 2014, 2015 Matthew Harvey

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
