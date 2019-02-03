# RegEx, a C regular expression interpreter

This is a personal project aimed at creating a regular expression interpreter
for C. There are two ultimate goals of this project:

 1. Implement [Perl Compatible Regular Expressions (PCRE)](http://www.pcre.org/).
 2. Create a library that can be used to write a web application in C.

## Building the Project

As I am now (and for the foreseeable future) on Windows, I've only setup a build
system for Windows. This takes the form of a batch script named `build.bat`.
Prerequisites for building this project are:

 -  Visual Studio 2017 (for using the debugger only)
 -  LLVM Clang
 -  A Windows/DOS compatible command-line

To run the build, first enable the Visual Studio environment with
`vcvarsall.bat` with the argument `amd64`. Then you can run `build.bat` from the
root of the project directory. This will create a `build\` directory in your
project that contains the project executable as well as debug info for Visual
Studio.

## Debugging the Project

To debug the project, run a new build and then run `devenv` on the executable
placed in the `build\` directory. This will open Visual Studio, which you can
use to debug the project.
