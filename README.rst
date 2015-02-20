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

Overview
========

``swx`` is a command line application for keeping track of the amount of
time you spend on different activities.

The name "swx" stands for "stopwatch extended", reflecting that it works
essentially like a stopwatch that has been extended with various additional
features to support fine-grained recording and reporting on activities
conducted over a period time.

Usage
=====

Basic usage
-----------

To use ``swx``, you enter a brief "switching" command each time you start an
activity, end an activity, or switch from one activity to another. ``swx``
makes a timestamped record of each such "transition" in a plain text file - which
you are free to peruse and edit. Then when you want a summary of how you have
spent your time, enter one of the reporting commands - which provide various
filtering and output options - and ``swx`` will analyze the text file and
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
are working on this activity (it will protest if you try to "open" a new activity
without this option; this guards against error in case you're trying to enter an
activity that has already been opened, but misspell it). On subsequent occasions,
when you switch back to an already-used activity, you would omit the ``-c`` (and
again ``swx`` will helpfully protest in case you think you are reusing an
existing activity, but aren't.)

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

If you pass the ``-r`` option to ``swx switch``, then the activity argument will
be treated as a regular expression (of the POSIX extended variety), rather
than an exact activity name. A switch will then be recorded to the most recently
active activity the name of which matches that regular expression. This can save
a fair bit of typing when switching back to a recently used activity.

Note activity names are case-sensitive.

The "resume" command
--------------------

Suppose you are currently "inactive" - on a lunch break, let's say - and then
you return to work and want to resume the most recent activity you were working
on before your break. Enter ``swx resume`` to record a resumption of the
activity you were working on just before the break. This is equivalent to
entering ``swx switch`` together with the name of the most recent activity.

If you are currently "active", then ``swx resume`` will record a switch to
the activity that was active just before the current one. This is useful for
when you are working on one activity, are briefly interrupted by another
activity, and then want to resume work on the original activity.

Reporting commands
------------------

To output a summary of the time you have spent on your various activities,
various "reporting commands" are available::

    swx print
    swx since
    swx between
    swx until
    swx day

Enter ``swx help <COMMAND>`` for detailed usage information in regards to
each of these. They all follow a similar pattern, and allow you to enter an
optional activity name, if you want to see only time spent on a given activity,
or to omit the activity name, if you want to see time spent on all activities.

``swx day`` (or ``swx d``) prints a summary of only the current day's
activities, or, if passed a single-digit option *n*, the activities of *n* days
ago. For example, ``swx day -1`` prints a summary of yesterday's activities.

``swx print`` (or ``swx p``) prints a summary of activity that is not filtered
by time at all.

The other printing commands take one or two timestamps as arguments, to show
only activity *since* a certain time, only activity *until* a certain time, or
only activity *between* two times. The activity name (if any) is entered after
the timestamp(s). By default, you must enter timestamps in ISO format
("YYYY-MM-DDTHH:MM") (without quotes). (You can customize this format in the
configuration file - see Configuration_ below - but note that changing this
format will also change the format of timestamps as read from and written to
the data file, *without* retroactively reformatting the timestamps that are
already stored - with resulting parsing errors unless you are prepared to
reformat manually all your already-entered timestamps to the new format.)

By default, only a summary of activities is printed. This comprises an
alphabetically ordered list of activities with the total amount of time spent
for each activity, during the period in question, shown in terms of digital
hours.

If you pass the ``-l`` option to the reporting command, then instead a list of
individual activity stints will be shown, in the following format:

    ----------  --------  -----------------------------  -------------
    start-time  end-time  hours-spent-in-digital-format  activity-name
    ----------  --------  -----------------------------  -------------

If you pass the ``-v`` option, then both the detailed list and the summary will
be printed.

If you pass the ``-r`` option, then the activity string you enter will be
treated as a regular expression (of the POSIX extended variety), rather than an
exact activity name. Any activities will then be included in the report for
which their activity name matches this regular expression.

If you pass the ``-b`` option, then in addition to the other info, the
earliest time at which each activity was conducted during the period
in question, will be printed next to each activity. (This does not apply when
outputting in "list" mode.)

If you pass the ``-e`` option, then in addition to, and to the right of,
any the other info, the latest time at which each activity was conducted during
the period in question, will be printed next to each activity. (This does not
apply when ouputting in "list" mode.)

Note that if ``-b`` and ``-e`` options are both provided, the output from
the ``-e`` command is always printed to the right of that from the ``-b``
command, regardless of the order in which the ``-b`` and ``-e`` options are
provided.

If you pass the ``-c`` option to a reporting command, then the results will
be output in CSV format.

By default, the number of hours shown is rounded to the nearest quarter of
an hour. This rounding behaviour can be changed in the Configuration_.

Complex activities
------------------

Activities are often divided conceptually into sub-activities,
sub-sub-activities and so forth. ``swx`` tries to capture this with the
concept of simple and compound activities. A simple activity is specified
using a single word, not containing whitespace, e.g. ``email``.
A compound activity is specified as multiple words separated by whitespace,
e.g. ``email customer-service``.

When passing the name of a compound activity to a ``swx`` command, it can
just be passed directly as multiple arguments to the command, without
enclosing it in quotes. ``swx`` will treat it as single, compound activity.
E.g., entering ``swx switch email customer-service`` is exactly equivalent
to entering ``swx switch 'email customer-service'``.

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
activities. E.g., if we are currently active on ``email customer-service
enquiries`` and want to record a switch to ``email customer-service
complaints``, then we can enter simply ``swx s __ complaints``, rather than
having to enter ``swx s email customer-service complaints``.

Manually editing the time log
-----------------------------

``swx`` stores a log of your activities in a plain text file, which by default
is located in your home directory, and is named ``<YOUR-USER-NAME>.swx``.
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

Configuration
-------------

Configuration options are stored in your home directory in the file named
``.swxrc``, which will be created the first time you run the program. The
contents of this file should be reasonably self-explanatory.

The command ``swx config`` will output a summary of your configuration settings.
Passing ``-e`` to this command will cause the configuration file to be opened
in your default text editor.

Help and other commands
-----------------------

Enter ``swx help`` to see a summary of usage, or ``swx help <COMMAND>`` to
see a summary of usage for a particular command.

Enter ``swx version`` to see version information.

Enter ``swx current`` (or ``swx c``) to print just the name of the current
activity. If there is no current activity, this will print a blank line.

Building and installing
=======================

``swx`` is written in standard C++, and uses some C++11 features. It is designed
to be built and run on Unix-like systems only (Linux, OSX, BSD), and will not
work on Windows. To build it, you will need:

- A reasonably conformant C++ compiler and standard library implementation (note
  if you are using GCC, you will need at least version 4.9)

- CMake (http://www.cmake.org) (commonly available via package managers such
  as Homebrew)

Having obtained these dependencies, download and unzip the ``swx`` source code,
and ``cd`` into the project root.

To configure an optimized build, enter::

   cmake -D CMAKE_BUILD_TYPE=Release .

(Note the dot at the end.) (For other build options, see the CMake documentation.)
Then to build and install, enter::

    make install

You may need to run this as root (e.g. by prefixing the above command with
``sudo``), depending on your system and the installation directory.

Uninstalling
============

When you run ``make install``, a file named ``install_manifest.txt`` will be
created in the source directory. This file contains a list of all files
installed by ``make install``. To uninstall ``swx``, you need manually to
remove each of the files in this list (of which there may well be only one).

In addition, the first time you run ``swx``, it will create a configuration
file called ``.swxrc``, in your home directory. Also, the first time you run
``swx switch`` (or ``swx s``), it will create a data file, in which your
activity log will be stored. Unless you have specified otherwise in your
configuration file, this data file will be stored in your home directory, and
will be named ``<YOUR-USER-NAME>.swx``. You may or may not want to remove this
file if you uninstall ``swx``.

Contact
=======

You are welcome to contact me about this project at:

software@matthewharvey.net
