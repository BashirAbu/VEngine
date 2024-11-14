@echo off
set build_type=%1

mkdir bin\int > NUL

set COMMON_FLAGS=/EHsc /utf-8 /std:c++20 /MP /nologo /W3 /wd4251 /wd4996
set DEBUG_FLAGS=/Z7 /Od /MDd
set RELEASE_FLAGS=/O2 /MD
set LINKER_FLAGS=/SUBSYSTEM:CONSOLE
set LIBS=Comdlg32.lib Gdi32.lib User32.lib Shell32.lib Winmm.lib bin\imgui\imgui.lib
set DEBUG_LIBS=third_party\raylib\build\raylib\Debug\raylib.lib
set RELEASE_LIBS=third_party\raylib\build\raylib\Release\raylib.lib
set LIB_NAME=VEngine.dll
set LIBS_FLAGS=/SUBSYSTEM:CONSOLE
set LINKER_FLAGS_COMMON=/NOLOGO /DLL
set INCLUDES=/I src /I third_party\raylib\src\ /I third_party\nlohmann_json\include\ /I third_party\imgui\ /I third_party\raygui\src\ /I third_party\rlImGui\ /I third_party\glm\ /I third_party\ImGuizmo\
set DEFINES=/DVE_EXPORT /DUSE_LIBTYPE_SHARED /DIMGUI_EXPORT /DBUILD_LIBTYPE_SHARED
set SRC=src\*.cpp src\platform\win32\*.cpp src\components\*.cpp src\entities\*.cpp src\editor\*.cpp src\scenes\*.cpp src\utils\*.cpp

call vcvarsall.bat x64 > NUL

del bin\*.dll 2> NUL
del bin\*.pdb 2> NUL
del bin\int\*.obj 2> NUL

if "%build_type%"=="Debug" (
    del bin\int\*.obj 2> NUL
    cl.exe /c %SRC% /Fo:bin\int\ %DEFINES% %COMMON_FLAGS% %DEBUG_FLAGS% %INCLUDES% 
    link.exe %LINKER_FLAGS_COMMON% /DEBUG /OUT:bin\%LIB_NAME% bin\int\*.obj %DEBUG_LIBS% %LIBS%
)
if "%build_type%"=="Release" (
    del bin\int\*.obj 2> NUL
    cl.exe /c %SRC% /Fo:bin\int\ %DEFINES% %COMMON_FLAGS% %RELEASE_FLAGS% %INCLUDES% 
    link.exe %LINKER_FLAGS_COMMON% /OUT:bin\%LIB_NAME% bin\int\*.obj %RELEASE_LIBS% %LIBS%
)
if "%build_type%"=="Clean" (
    del bin\int\*.obj 
)

