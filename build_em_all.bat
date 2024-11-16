@echo off
set build_type=%1
cd VEngine\third_party\raylib
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF -DBUILD_GAMES=OFF -DBUILD_TESTING=OFF ..
cmake --build . --config %build_type%
cd ..
cd ..
cd glfw
mkdir build
cd build
cmake ..
cmake --build . --config %build_type%
cd ..
cd ..
cd ..
call build_imgui.bat %build_type%
call build.bat %build_type%
cd ..
cd Fusion
call build.bat %build_type%
pause