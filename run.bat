@echo off
setlocal

cd /D "%~dp0"

if not exist "bin" (
    echo "Error: 'bin' directory does not exist. Please run build.bat first."
    exit /b 1
)

call bin\Trace.exe

endlocal