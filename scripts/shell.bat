@echo off

rem
rem @author Joseph Miles <josephmiles2015@gmail.com>
rem @date 2019-02-03
rem
rem This is my personal shell setup script.
rem

echo Adding scripts directory to path
rem NOTE[joe] %~dp0 grabs the directory where this script lives.
path %PATH%;%~dp0

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
