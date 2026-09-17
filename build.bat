@echo off
setlocal

cd /D "%~dp0"

if not exist "bin" (
    mkdir bin
)

pushd bin

set FLAGS=/std:c++20 /FC /EHsc /MP /Zi /W4 /MD
rem set FLAGS=/std:c++20 /O2 /MP /GL

set INCLUDE_PATH=/I"..\dependencies\raylib\include"
set LIB_PATH=..\dependencies\raylib\lib
set DEPENDENCY_LIB=raylib.lib

set SYSTEM_LIBS=User32.lib Gdi32.lib Shell32.lib Winmm.lib

set SOURCE_FILES=..\source\main.cpp ..\source\game.cpp
set OUTPUT_EXE=Trace.exe

cl %FLAGS% %INCLUDE_PATH% %SOURCE_FILES% %SYSTEM_LIBS% ^
    /link /LIBPATH:"%LIB_PATH%" %DEPENDENCY_LIB% /OUT:"%OUTPUT_EXE%"

popd
endlocal