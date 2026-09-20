@echo off
setlocal

cd /D "%~dp0"

if not exist "dependencies" ( 
    echo ERROR: Dependencies not found.
    exit /b 1 
)

if not exist "bin" ( mkdir bin )

pushd bin 

@REM ----------------------------------------------------------
echo Compiling Dependencies...

pushd ..\dependencies\raygui 
if not exist "lib" (mkdir lib)
pushd lib

call cl /nologo /c /O2 /DNDEBUG /MD /I"..\..\raylib\include" ..\include\raygui.c /Fo:raygui.obj 
call lib /nologo raygui.obj /OUT:raygui.lib

popd
popd

echo Done.
@REM ----------------------------------------------------------



set FLAGS=/std:c++20 /FC /EHsc /MP /Zi /W4 /MD
rem set FLAGS=/std:c++20 /O2 /MP /GL

set INCLUDE_PATH=/I"..\dependencies\raylib\include" /I"..\dependencies\raygui\include"
set LIB_PATH=/LIBPATH:"..\dependencies\raylib\lib" /LIBPATH:"..\dependencies\raygui\lib"
set DEPENDENCY_LIB=raylib.lib raygui.lib

set SYSTEM_LIBS=User32.lib Gdi32.lib Shell32.lib Winmm.lib

set SOURCE_FILES=..\source\main.cpp ..\source\game.cpp
set OUTPUT_EXE="Trace.exe"

echo Compiling Game...
call cl /nologo %FLAGS% %INCLUDE_PATH% %SOURCE_FILES% %SYSTEM_LIBS% ^
        /link %LIB_PATH% %DEPENDENCY_LIB% /OUT:%OUTPUT_EXE%
echo Done.

popd
endlocal