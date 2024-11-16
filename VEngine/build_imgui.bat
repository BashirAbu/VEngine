@echo off
set build_type=%1
mkdir bin\imgui\
set COMMON_FLAGS=/EHsc /utf-8 /std:c++20 /MP /nologo /W3 /wd4251 /wd4996
set DEBUG_FLAGS=/Z7 /O1 /MDd
set RELEASE_FLAGS=/O2 /MD
set LIBS=Gdi32.lib
set DEBUG_LIBS=third_party\glfw\build\src\Debug\glfw3.lib
set RELEASE_LIBS=third_party\glfw\build\src\Release\glfw3.lib
set LIB_NAME=imgui.dll
set LINKER_FLAGS_COMMON=/NOLOGO /DLL
set INCLUDES=/I third_party\imgui\ /I third_party\glfw\include /I third_party\ImGuizmo\
set DEFINES=/DIMGUI_EXPORT
set SRC=third_party\imgui\imgui.cpp third_party\imgui\imgui_draw.cpp third_party\imgui\imgui_tables.cpp third_party\imgui\imgui_widgets.cpp third_party\imgui\imgui_demo.cpp third_party\imgui\backends/imgui_impl_opengl3.cpp third_party\ImGuizmo\ImGuizmo.cpp

call vcvarsall.bat x64 > NUL


if "%build_type%"=="Debug" (
    cl.exe /c %SRC% /Fo:bin\imgui\ %DEFINES% %COMMON_FLAGS% %DEBUG_FLAGS% %INCLUDES% 
    link.exe %LINKER_FLAGS_COMMON% /DEBUG /OUT:bin\imgui\%LIB_NAME% bin\imgui\*.obj %DEBUG_LIBS% %LIBS%
)
if "%build_type%"=="Release" (
    
    cl.exe /c %SRC% /Fo:bin\imgui\ %DEFINES% %COMMON_FLAGS% %RELEASE_FLAGS% %INCLUDES% 
    link.exe %LINKER_FLAGS_COMMON% /OUT:bin\imgui\%LIB_NAME% bin\imgui\*.obj %RELEASE_LIBS% %LIBS%
)
