Legal
=====

Copyright 2014 Matthew Harvey

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

``swx`` is simple command line application for keeping track of the amount of
time you spend on different activities.

Usage
=====

Basic usage
-----------

``swx`` provides a series of commands, of which you can see a list by
entering ``swx help``. The basic pattern of usage is::

    swx <COMMAND> [ARGUMENTS]

The "switch" command
--------------------

Suppose you start working on the activity of "answering emails". You would come
up with a name for this activity, say ``answering emails``. When you first start
working on this activity, you would enter the following at the command line::

    swx switch -c answering emails

The activity name can be any number of words strung together, and is
case-sensitive. You can use the alias ``sw`` if you don't want to type
``switch``.

The ``-c`` option tells the ``switch`` command that this is the first time you
are working on this activity. On subsequent occasions, when you switch back to
an already-used activity, you should omit the ``-c`` option.

Suppose you stop answering emails and restart work on a previous activity,
"spreadsheeting". You can record a transition from one activity to another, by
entering ``swx switch``, plus the name of the acitivity that you are switching
*to*, in this case::

    swx switch spreadsheeting

If you cease doing any activity at all (or at least, any activity you care about
recording), then you would record this cessation by entering simply::
    
    swx switch

The "resume" command
--------------------

Suppose you are currently "inactive" - on a lunch break, let's say - and then
you return to work and want to resume the most recent activity you were working
on before your break. Enter ``swx resume`` (or simply ``swx r``) to record a
resumption of the activity you were working on just before the break. This is
equivalent to entering ``swx switch`` together with the name of the most recent
activity.

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
    swx today

Enter ``swx help <COMMAND>`` for detailed usage information in regards to
each of these. They all follow a similar pattern, and allow you to enter an
optional activity name, if you want to see only time spent on a given activity,
or to omit the activity name, if you want to see time spent on all activities.

``swx today`` prints a summary of only the current day's activities.

``swx print`` prints a summary of activity that is not filtered by time at all.

The other printing commands take one or two timestamps as arguments, to show
only activity *since* a certain time, only activity *until* a certain time, or
only activity *between* two times. The activity name (if any) is entered after
the timestamp(s). By default, you must enter timestamps in ISO format
("YYYY-MM-DDTHH:MM:SS") (without quotes). (You can customize this format in the
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

    start-time  end-time  hours-spent-in-digital-format  activity-name

If you pass the ``-v`` option, then both the detailed list and the summary will
be printed.

If you pass the ``-r`` option, then the activity string you enter will be
treated as a regular expression (of the POSIX extended variety), rather than an
exact activity name. Any activities will then be included in the report for
which their activity name matches this regular expression.

By default, the number of hours shown is rounded to the nearest quarter of
an hour. This rounding behaviour can be changed in the Configuration_.

Manually editing the time log
-----------------------------

``swx`` stores a log of your activities in a plain text file, which by default
is located in your home directory, and is named ``<YOUR-USER-NAME>.swx``.
You are free to edit this file in a text editor if you want to change the
times or activity names recorded. Be sure to preserve the prescribed timestamp
format, and to leave a space between the timestamp and the activity name
(if any) on any given line. (Lines without an activity name record a cessation
of activity.) Also, the time log must be such that the timestamps appear in
ascending order (or at least, non-descending order). Be sure to preserve this
order if you edit the file manually.

Help
----

Enter ``swx help`` to see a summary of usage, or ``swx help <COMMAND>`` to
see a summary of usage for a particular command.

Enter ``swx version`` to see version information.

Configuration
=============

Configuration options are stored in your home directory in the file named
``.swxrc``, which will be created the first time you run the program. The
contents of this file should be reasonably self-explanatory.

Building and installing
=======================

``swx`` is written in standard C++, and uses some C++11 features. It is designed
to be built and run on Unix-like systems only (Linux, OSX, BSD), and will not
work on Windows. To build it, you will need:

- A reasonably conformant C++ compiler and standard library implementation

- CMake (version 2.8.10 or later) (http://www.cmake.org)

- The Boost regex library (version 1.53 or later) (http://www.boost.org)

Download and unzip the source code and ``cd`` into the source root.

Enter::

	cmake -i

You will be prompted with ``Would you like to see the advanced options? [No]:``.
Enter ``n``. Then follow the prompts.

(If you run into trouble with the build or have atypical requirements, you can
return to this step and answer ``y`` to configure more detailed build options.)

If in doubt about a particular option, it is generally best just to hit enter
and keep the default setting for the option.

Depending on your compiler, you may need to set an option to build with
C++11 support enabled.

Once you have finished configuring the build with CMake, enter::

	make install

You may need to run this as root, i.e.::

	sudo make install

To clean build
--------------

Go to the project root and enter::
	
	make clean

This will clean all build targets from the project root.

Note this will *not* cause the application to be uninstalled from the host
system.

To build without installing
---------------------------

At the project root, enter::

	make

Uninstalling
============

When you run ``make install``, a file named ``install_manifest.txt`` will be
created in the source directory. This file contains a list of all files
installed by ``make install``. To uninstall ``swx``, you need manually to
remove each of the files in this list (of which there may well be only one).

In addition, the first time you run ``swx``, it will create a configuration
file called ``.swxrc``, in your home directory. Also, the first time you run
``swx switch`` (or ``swx sw``), it will create a data file, in which your
activity log will be stored. Unless you have specified otherwise in your
configuration file, this data file will be stored in your home directory, and
will be named ``<YOUR-USER-NAME>.swx``. You may or may not want to remove this
file if you uninstall ``swx``.

Contact
=======

You are welcome to contact me about this project at:

software@matthewharvey.net
