@echo off

REM Fix "Run As Administrator" cwd
setlocal EnableExtensions
cd /d "%~dp0"

REM Determine if the user is an administrator or not.
REM Administrators will install for all users, normal users won't.

net session >NUL 2>&1
if %ERRORLEVEL% == 0 (
  REM Administrator
  set install=%ProgramFiles%\GIMP 2\lib\gimp\2.0\plug-ins
  echo.
  echo This will install the GIMP 2.8 XYZ plugin v0.1 for ALL users.
  echo.
  echo It assumes that GIMP 2.8 is installed at:
  echo %ProgramFiles%\GIMP 2
  echo.
  echo If you're unsure of where the GIMP is installed,
  echo please run this as a normal user!
  echo.
) else (
  REM Normal User
  set install=%USERPROFILE%\.gimp-2.8\plug-ins
  echo.
  echo This will install the GIMP 2.8 XYZ plugin v0.1 for the user %USERNAME%.
  echo If you would like to install for all users, run this file as an administrator.
  echo.
)

:choice
echo Continue? (y/n)
choice /c:yn > nul
if errorlevel 2 goto :EOF
if errorlevel 1 goto :install
goto :choice

:install

REM Determine if this is an x86 or x64 OS.
if defined ProgramFiles(x86) (
  set arch=64
) else (
  set arch=32
)

mkdir "%install%" 2>NUL
copy /b/v/y bin\win\%arch%\file-xyz.exe "%install%" >NUL
if %ERRORLEVEL% == 0 (
  echo Plugin successfully installed! You'll need to restart the GIMP to use it.
  set rc=0
) else (
  echo Something went wrong! Did you extract the entire contents of the ZIP archive?
  set rc=1
)
echo.
pause
exit /B %rc%