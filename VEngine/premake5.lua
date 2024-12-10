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
        symbols "On"
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
    

        
    filter "configurations:Release"
    staticruntime "On"
    optimize "On"
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
    

project "VEngine"

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
        systemversion "latest"
        buildoptions "/utf-8 /MP /nologo /W3 /wd4251 /wd4996 /wd4005 /wd4002"
        defines
        {
            "VE_WINDOWS"
        }
        prebuildcommands
        {
            "{CHDIR} %{prj.location}",
            "%{wks.location}/bin/" .. outputDir .. "/VEHeaderTool/VEHeaderTool.exe e  src/generated/ src/components/ -c VE_CLASS -e VE_ENUM -f VE_FUNCTION -p VE_PROPERTY"
        }
        filter "configurations:Debug"
        kind "SharedLib"
        staticruntime "Off"
        symbols "On"
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
            "freetyped",
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
            "VE_EXPORT",
            "USE_LIBTYPE_SHARED",
            "BUILD_LIBTYPE_SHARED",
            "flecs_EXPORTS",
        }
        prebuildcommands
        {
            "{CHDIR} %{prj.location}",
            "%{wks.location}/bin/" .. outputDir .. "/VEHeaderTool/VEHeaderTool.exe e  src/generated/ src/components/ -c VE_CLASS -e VE_ENUM -f VE_FUNCTION -p VE_PROPERTY"
        }

    filter "configurations:Release"
        kind "SharedLib"
        staticruntime "On"
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
            "VE_EXPORT",
            "USE_LIBTYPE_SHARED",
            "BUILD_LIBTYPE_SHARED",
            "flecs_EXPORTS"
        }
        optimize "On"
        prebuildcommands
        {
            "{CHDIR} %{prj.location}",
            "%{wks.location}/bin/" .. outputDir .. "/VEHeaderTool/VEHeaderTool.exe e  src/generated/ src/components/ -c VE_CLASS -e VE_ENUM -f VE_FUNCTION -p VE_PROPERTY"
        }

    filter "configurations:Game_Debug"
        kind "StaticLib"
        staticruntime "Off"
        defines {"VE_DEBUG", "flecs_STATIC"}
        symbols "On"
        libdirs
        {
            "%{prj.location}/third_party/raylib/build/raylib/Debug/",
            "%{prj.location}/third_party/freetype/build/Debug/"
        }
        links
        {
            "raylib",
            "freetyped"
        }


    filter "configurations:Game_Release"
        kind "StaticLib"
        staticruntime "On"
        defines {"VE_RELEASE", "flecs_STATIC"}
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

