@echo off
set build_type=%1
mkdir bin\int
set FLAGS=/nologo /EHsc /utf-8 /std:c++17 /MP /W3 /wd4312 /wd4005 /wd4172 /wd4127 /wd4530 /wd4459 /wd5105 /wd4100 /wd4996 /wd4251
set LIB_NAME=test_project.dll
set LIBS=..\VEngine\bin\VEngine.lib Gdi32.lib User32.lib Shell32.lib Winmm.lib
set DEBUG_LIBS=..\VEngine\third_party\raylib\build\raylib\Debug\raylib.lib
set RELEASE_LIBS=..\VEngine\third_party\raylib\build\raylib\Release\raylib.lib
set INCLUDES=/I src /I ..\VEngine\src /I ..\VEngine\third_party\imgui\ /I ..\VEngine\third_party\raylib\build\raylib\include /I ..\VEngine\third_party\glm\ /I ..\VEngine\third_party\nlohmann_json\include\
set DEFINES=/DVE_PROJECT_EXPORT
set SRC=src\*.cpp

:: Get the current date and time
for /f "tokens=2-4 delims=/- " %%a in ('date /t') do (
    set month=%%a
    set day=%%b
    set year=%%c
)

for /f "tokens=1-4 delims=:." %%a in ('echo %time%') do (
    set hour=%%a
    set minute=%%b
    set second=%%c
)

:: Format the date and time as YYYYMMDDHHMMSS
set formatted_datetime=%year%%month%%day%%hour%%minute%%second%


call vcvarsall.bat x64 > NUL

if "%build_type%"=="Debug" (
    del bin\*.pdb 2> NUL
    cl.exe /c /Fo:bin\int\ %SRC% /Z7 /Od /MDd  %FLAGS% %INCLUDES% %DEFINES%
    link.exe /MAP /DEBUG /DLL /NOLOGO /PDB:bin\test_project_%formatted_datetime%.pdb /OUT:bin\%LIB_NAME% bin\int\*.obj %DEBUG_LIBS% %LIBS%
)

if "%build_type%"=="Release" (
    cl.exe /c /Fo:bin\int\ %SRC% /O2 /MD  %FLAGS% %INCLUDES% %DEFINES%
    link.exe /MAP /DLL /NOLOGO /OUT:bin\%LIB_NAME% bin\int\*.obj %RELEASE_LIBS% %LIBS%
)
