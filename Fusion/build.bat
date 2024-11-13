@echo off
set build_type=%1
mkdir bin\int 2> NUL
set COMMON_FLAGS=/EHsc /utf-8 /std:c++20 /MP /nologo /W3 /wd4251
set DEBUG_FLAGS=/Z7 /Od /MDd
set RELEASE_FLAGS=/O2 /MD
set LINKER_FLAGS=/SUBSYSTEM:CONSOLE
set LIBS=..\VEngine\bin\VEngine.lib ..\VEngine\bin\imgui\imgui.lib
set DEBUG_LIBS=..\VEngine\third_party\raylib\build\raylib\Debug\raylib.lib
set RELEASE_LIBS=..\VEngine\third_party\raylib\build\raylib\Release\raylib.lib
set EXE_NAME=Fusion.exe
set LIBS_FLAGS=/SUBSYSTEM:CONSOLE
set INCLUDES=/I src /I ..\VEngine\src\ /I ..\VEngine\third_party\raylib\build\raylib\include /I ..\VEngine\third_party\nlohmann_json\include\ /I ..\VEngine\third_party\glm\ /I ..\VEngine\third_party\imgui\ /I ..\VEngine\third_party\rlImGui\
set DEFINES=/DUSE_LIBTYPE_SHARED
set SRC=src\*.cpp
if not defined DevEnvDir (
    call vcvarsall.bat x64 > NUL
)


del bin\*.dll 2> NUL
del bin\*.pdb 2> NUL
del bin\int\*.obj 2> NUL


if "%build_type%"=="Debug" (

    cl.exe  %SRC% /Fe:bin\%EXE_NAME% /Fo:bin\int\ %DEFINES% %COMMON_FLAGS% %DEBUG_FLAGS% %INCLUDES% /link %DEBUG_LIBS% %LIBS% %LINKER_FLAGS% 
    copy ..\VEngine\third_party\raylib\build\raylib\Debug\raylib.dll bin\ > NUL
    copy ..\VEngine\third_party\raylib\build\raylib\Debug\raylib.pdb bin\ > NUL
    copy ..\VEngine\bin\VEngine.dll bin\ > NUL
    copy ..\VEngine\bin\VEngine.pdb bin\ > NUL
    copy ..\VEngine\bin\imgui\imgui.dll bin\ > NUL
    copy ..\VEngine\bin\imgui\imgui.pdb bin\ > NUL
)

if "%build_type%"=="Release" (
    cl.exe %SRC% /Fo:bin\int\  /Fe:bin\%EXE_NAME% %DEFINES% %COMMON_FLAGS% %RELEASE_FLAGS% %INCLUDES% /link %RELEASE_LIBS% %LIBS% %LINKER_FLAGS%
    copy ..\VEngine\third_party\raylib\build\raylib\Release\raylib.dll bin\ > NUL
    copy ..\VEngine\bin\VEngine.dll bin\ > NUL
    copy ..\VEngine\bin\imgui\imgui.dll bin\ > NUL
)

if "%build_type%"=="MemoryCheck" (
    heob64.exe bin\%EXE_NAME% -p ..\test_project\test_project.VEProject
)