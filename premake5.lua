workspace "VEngine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputDir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"
project "ImGui"
    location "VEngine"
    kind "SharedLib"
    language "c++"

    targetdir("bin/" .. outputDir .. "/%{prj.name}")
    objdir("bin_int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "%{prj.location}/third_party/imgui/imgui.cpp",
        "%{prj.location}/third_party/imgui/imgui_draw.cpp",
        "%{prj.location}/third_party/imgui/imgui_tables.cpp",
        "%{prj.location}/third_party/imgui/imgui_widgets.cpp",
        "%{prj.location}/third_party/imgui/imgui_demo.cpp",
        "%{prj.location}/third_party/imgui/backends/imgui_impl_opengl3.cpp",
        "%{prj.location}/third_party/imgui/backends/imgui_impl_glfw.cpp",
        "%{prj.location}/third_party/ImGuizmo/ImGuizmo.cpp"  
    }

    includedirs
    {
        
        "%{prj.location}/third_party/imgui/",
        "%{prj.location}/third_party/glfw/include",
        "%{prj.location}/third_party/ImGuizmo/"
    }
    
    libdirs
    {
        "%{prj.location}/third_party/glfw/build/src/%{cfg.buildcfg}/"
    }
    links
    {
        "glfw3"
    }


    filter "system:windows"
        cppdialect "C++20"
        staticruntime "off"
        systemversion "latest"

        defines
        {
            "IMGUI_EXPORT"
        }
        
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/glfw/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake ..",
            "cmake --build . --config %{cfg.buildcfg}"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Fusion")
        }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

project "Fusion"
    location "Fusion"
    kind "ConsoleApp"
    language "c++"

    targetdir("bin/" .. outputDir .. "/%{prj.name}")
    objdir("bin_int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "%{prj.location}/src/**.h",     
        "%{prj.location}/src/**.hpp",      
        "%{prj.location}/src/**.cpp",      
        "%{prj.location}/src/**.c"    
    }

    includedirs
    {
        "%{prj.location}/src",
        "%{wks.location}/VEngine/src/",
        "%{wks.location}/VEngine/third_party/raylib/build/raylib/include",
        "%{wks.location}/VEngine/third_party/nlohmann_json/include/",
        "%{wks.location}/VEngine/third_party/glm/",
        "%{wks.location}/VEngine/third_party/imgui/",
        "%{wks.location}/VEngine/third_party/rlImGui/",
    }
    
    libdirs
    {
        "%{wks.location}/VEngine/third_party/raylib/build/raylib/%{cfg.buildcfg}/"
    }
    links
    {
        "raylib"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "off"
        systemversion "latest"
        buildoptions "/MP /nologo /W3 /wd4251 /wd4996 /wd4005"
        defines
        {
            "VE_WINDOWS",
            "USE_LIBTYPE_SHARED"
        }

        libdirs 
        {
            "%{wks.location}/bin/" .. outputDir .. "/ImGui/",
            "%{wks.location}/bin/" .. outputDir .. "/VEngine/"
            
        }

        links
        {
            "ImGui.lib",
            "VEngine.lib"
        }
    
    filter "configurations:Debug"
        defines "VE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "VE_RELEASE"
        optimize "On"







project "VEngine"
    location "VEngine"
    kind "SharedLib"
    language "c++"

    targetdir("bin/" .. outputDir .. "/%{prj.name}")
    objdir("bin_int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "%{prj.location}/src/**.h",     
        "%{prj.location}/src/**.hpp",      
        "%{prj.location}/src/**.cpp",      
        "%{prj.location}/src/**.c"    
    }

    includedirs
    {
        "%{prj.location}/src",
        "%{prj.location}/third_party/raylib/src/",
        "%{prj.location}/third_party/nlohmann_json/include/",
        "%{prj.location}/third_party/imgui/",
        "%{prj.location}/third_party/raygui/src/",
        "%{prj.location}/third_party/rlImGui/",
        "%{prj.location}/third_party/glm/",
        "%{prj.location}/third_party/ImGuizmo/",
        "%{prj.location}/third_party/glfw/include"
    }
    
    libdirs
    {
        "%{prj.location}/third_party/raylib/build/raylib/%{cfg.buildcfg}/",
        "%{prj.location}/third_party/glfw/build/src/%{cfg.buildcfg}/"
    }
    links
    {
        "raylib",
        "glfw3"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "off"
        systemversion "latest"
        buildoptions "/MP /nologo /W3 /wd4251 /wd4996 /wd4005"
        defines
        {
            "VE_WINDOWS",
            "VE_EXPORT",
            "USE_LIBTYPE_SHARED",
            "BUILD_LIBTYPE_SHARED"
        }

        libdirs 
        {
            "%{wks.location}/bin/" .. outputDir .. "/ImGui/"
            
        }

        links
        { 
            "ImGui.lib"
        }

        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/raylib/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF -DBUILD_GAMES=OFF -DBUILD_TESTING=OFF ..",
            "cmake --build . --config %{cfg.buildcfg}"
        }
        postbuildcommands
        {
            "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Fusion",
            "{COPY} \"%{wks.location}/VEngine/third_party/raylib/build/raylib/%{cfg.buildcfg}/raylib.dll\" ../bin/" .. outputDir .. "/Fusion"
        }
    
    filter "configurations:Debug"
        defines "VE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "VE_RELEASE"
        optimize "On"