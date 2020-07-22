# RegEx, a C regular expression interpreter

This is a personal project aimed at creating a regular expression interpreter
for C. There are two ultimate goals of this project:

 1. Implement [Perl Compatible Regular Expressions (PCRE)](http://www.pcre.org/).
 2. Create a library that can be used to write a web application in C.

## Building the Project

As I am now (and for the foreseeable future) on Windows, I've only setup a build
system for Windows. This takes the form of a PowerShell script named `build.ps1`.
This script requires you to have the LLVM toolchain installed and added to your
path.
