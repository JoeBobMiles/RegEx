@echo off

rem
rem @author Joseph Miles <josephmiles2015@gmail.com>
rem @date 2019-02-03
rem
rem This is the build script for Windows, using clang-cl.
rem

mkdir build
pushd build
clang-cl /Zi ..\src\main.c ..\src\fa.c
popd
