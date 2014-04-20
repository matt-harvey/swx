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

It is still being written and is not yet ready for use.

Dependencies
============

``swx`` is written in standard C++, and uses some C++11 features. It is designed
to build and run on Unix-like systems only (Linux, OSX, BSD), and will not
work on Windows. To build it, you will need:

- A reasonably conformant C++ compiler and standard library implementation

- CMake (version 2.8 or later) (http://www.cmake.org)

Usage
=====

Enter ``swx help`` to get help.

*TODO* Expand on this.

Building and installing
=======================

Open a command line and ``cd`` to the project root.

Enter::

	cmake -i

You will be prompted with ``Would you like to see the advanced options? [No]:``.
Enter ``n``. Then follow the prompts.

(If you run into trouble with the build or have atypical requirements, you can
return to this step and answer ``y`` to configure more detailed build options.)

If in doubt about a particular option, it is generally best just to hit enter
and keep the default setting for the option.

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

*TODO*

Contact
=======

You are welcome to contact me about this project at:

software@matthewharvey.net
