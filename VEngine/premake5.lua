project "ImGui"
    kind "SharedLib"
    language "c++"

    targetdir("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir("%{wks.location}/bin_int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "%{prj.location}/third_party/imgui/imgui.cpp",
        "%{prj.location}/third_party/imgui/imgui_draw.cpp",
        "%{prj.location}/third_party/imgui/imgui_tables.cpp",
        "%{prj.location}/third_party/imgui/imgui_widgets.cpp",
        "%{prj.location}/third_party/imgui/imgui_demo.cpp",
        "%{prj.location}/third_party/imgui/backends/imgui_impl_opengl3.cpp",
        "%{prj.location}/third_party/ImGuizmo/ImGuizmo.cpp",
    }

    includedirs
    {
        
        "%{prj.location}/third_party/imgui/",
        "%{prj.location}/third_party/glfw/include",
        "%{prj.location}/third_party/ImGuizmo/",
        "%{prj.location}/third_party/ShapingEngine/",
    }
    
    links
    {
        "glfw3"
    }


    filter "system:windows"

        cppdialect "C++20"
        staticruntime "off"
        systemversion "latest"
        buildoptions "/utf-8 /MP /nologo /W3 /wd4251 /wd4996 /wd4005 /wd4002"
        defines
        {
            "IMGUI_EXPORT"
        }
        
       
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Fusion")
        }
    filter "configurations:Debug"
        libdirs
        {
            "%{prj.location}/third_party/glfw/build/src/%{cfg.buildcfg}/"
        }
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/glfw/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake ..",
            "cmake --build . --config %{cfg.buildcfg}"
        }
    

        symbols "On"
        
    filter "configurations:Release"
        libdirs
        {
            "%{prj.location}/third_party/glfw/build/src/%{cfg.buildcfg}/"
        }
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/glfw/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake ..",
            "cmake --build . --config %{cfg.buildcfg}"
        }
    

        optimize "On"

    filter "configurations:Game_Debug"
        libdirs
        {
            "%{prj.location}/third_party/glfw/build/src/Debug/"
        }
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/glfw/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake ..",
            "cmake --build . --config Debug"

        }

        defines "VE_DEBUG"
        symbols "On"

    filter "configurations:Game_Release"
        libdirs
        {
            "%{prj.location}/third_party/glfw/build/src/Release/"
        }
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/glfw/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake ..",
            "cmake --build . --config Release"
        }

        defines "VE_RELEASE"
        optimize "On"

project "VEngine"
kind "SharedLib"
language "c++"
    
    targetdir("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir("%{wks.location}/bin_int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "%{prj.location}/src/**.h",     
        "%{prj.location}/src/**.hpp",      
        "%{prj.location}/src/**.cpp",      
        "%{prj.location}/src/**.c",
        "%{prj.location}/third_party/flecs/distr/flecs.c",
    }

    includedirs
    {
        "%{prj.location}/src",
        "%{prj.location}/third_party/raylib/src/",
        "%{prj.location}/third_party/nlohmann_json/include/",
        "%{prj.location}/third_party/glm/",
        "%{prj.location}/third_party/flecs/include/",
        "%{prj.location}/third_party/freetype/include/",
        "%{prj.location}/third_party/ShapingEngine/",
    }
    


    vectorextensions "Default"

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "off"
        systemversion "latest"
        buildoptions "/utf-8 /MP /nologo /W3 /wd4251 /wd4996 /wd4005 /wd4002"
        defines
        {
            "VE_WINDOWS",
            "VE_EXPORT",
            "USE_LIBTYPE_SHARED",
            "BUILD_LIBTYPE_SHARED",
            "flecs_EXPORTS"
        }



    
    filter "configurations:Debug"
        includedirs
        {
            "%{prj.location}/third_party/imgui/",
            "%{prj.location}/third_party/raygui/src/",
            "%{prj.location}/third_party/rlImGui/",
            "%{prj.location}/third_party/glfw/include",
            "%{prj.location}/third_party/ImGuizmo/",
        }

        libdirs
        {
            "%{prj.location}/third_party/raylib/build/raylib/%{cfg.buildcfg}/",
            "%{prj.location}/third_party/freetype/build/%{cfg.buildcfg}/"
        }
        links
        {
            "raylib",
            "freetype"
        }

        libdirs 
        {
            "%{wks.location}/bin/" .. outputDir .. "/ImGui/"
            
        }

        links
        { 
            "ImGui.lib"
        }
        defines 
        {
            "VE_DEBUG",
            "VE_EDITOR",
        }
        symbols "On"
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/raylib/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF -DBUILD_GAMES=OFF -DBUILD_TESTING=OFF ..",
            "cmake --build . --config %{cfg.buildcfg}",

            "{CHDIR} %{prj.location}",
            "%{wks.location}/bin/" .. outputDir .. "/VEHeaderTool/VEHeaderTool.exe e  src/generated/ src/components/ -c VE_CLASS -e VE_ENUM -f VE_FUNCTION -p VE_PROPERTY"
        }
        postbuildcommands
        {
            "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Fusion",
            "{COPY} \"%{wks.location}/VEngine/third_party/raylib/build/raylib/%{cfg.buildcfg}/raylib.dll\" ../bin/" .. outputDir .. "/Fusion"
        }

    filter "configurations:Release"
        includedirs
        {
            "%{prj.location}/third_party/imgui/",
            "%{prj.location}/third_party/raygui/src/",
            "%{prj.location}/third_party/rlImGui/",
            "%{prj.location}/third_party/glfw/include",
            "%{prj.location}/third_party/ImGuizmo/",
        }
        libdirs
        {
            "%{prj.location}/third_party/raylib/build/raylib/%{cfg.buildcfg}/",
            "%{prj.location}/third_party/freetype/build/%{cfg.buildcfg}/"
        }
        links
        {
            "raylib",
            "freetype"
        }
        libdirs 
        {
            "%{wks.location}/bin/" .. outputDir .. "/ImGui/"
            
        }

        links
        { 
            "ImGui.lib"
        }

        defines 
        {
            "VE_RELEASE",
            "VE_EDITOR",
        }
        optimize "On"
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/raylib/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF -DBUILD_GAMES=OFF -DBUILD_TESTING=OFF ..",
            "cmake --build . --config %{cfg.buildcfg}",

            "{CHDIR} %{prj.location}",
            "%{wks.location}/bin/" .. outputDir .. "/VEHeaderTool/VEHeaderTool.exe e  src/generated/ src/components/ -c VE_CLASS -e VE_ENUM -f VE_FUNCTION -p VE_PROPERTY"
        }
        postbuildcommands
        {
            "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Fusion",
            "{COPY} \"%{wks.location}/VEngine/third_party/raylib/build/raylib/%{cfg.buildcfg}/raylib.dll\" ../bin/" .. outputDir .. "/Fusion"
        }

    filter "configurations:Game_Debug"
    
        defines "VE_DEBUG"
        symbols "On"
        libdirs
        {
            "%{prj.location}/third_party/raylib/build/raylib/Debug/",
            "%{prj.location}/third_party/freetype/build/Debug/"
        }
        links
        {
            "raylib",
            "freetype"
        }
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/raylib/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF -DBUILD_GAMES=OFF -DBUILD_TESTING=OFF ..",
            "cmake --build . --config Debug",

            "{CHDIR} %{prj.location}",
            "%{wks.location}/bin/" .. outputDir .. "/VEHeaderTool/VEHeaderTool.exe e  src/generated/ src/components/ -c VE_CLASS -e VE_ENUM -f VE_FUNCTION -p VE_PROPERTY"
        }
        postbuildcommands
        {
            "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Fusion",
            "{COPY} \"%{wks.location}/VEngine/third_party/raylib/build/raylib/Debug/raylib.dll\" ../bin/" .. outputDir .. "/Fusion"
        }

    filter "configurations:Game_Release"
    
        defines "VE_RELEASE"
        optimize "On"
        libdirs
        {
            "%{prj.location}/third_party/raylib/build/raylib/Release/",
            "%{prj.location}/third_party/freetype/build/Release/"
        }
        links
        {
            "raylib",
            "freetype"
        }
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/raylib/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF -DBUILD_GAMES=OFF -DBUILD_TESTING=OFF ..",
            "cmake --build . --config Release",

            "{CHDIR} %{prj.location}",
            "%{wks.location}/bin/" .. outputDir .. "/VEHeaderTool/VEHeaderTool.exe e  src/generated/ src/components/ -c VE_CLASS -e VE_ENUM -f VE_FUNCTION -p VE_PROPERTY"
        }
        postbuildcommands
        {
            "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Fusion",
            "{COPY} \"%{wks.location}/VEngine/third_party/raylib/build/raylib/Release/raylib.dll\" ../bin/" .. outputDir .. "/Fusion"
        }