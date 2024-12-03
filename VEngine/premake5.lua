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
        "%{prj.location}/third_party/ImGuizmo/ImGuizmo.cpp"  
    }

    includedirs
    {
        
        "%{prj.location}/third_party/imgui/",
        "%{prj.location}/third_party/glfw/include",
        "%{prj.location}/third_party/ImGuizmo/"
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
        
       

    filter "configurations:Debug"
        libdirs
        {
            "%{prj.location}/third_party/glfw/build/src/Debug/"
        }
        symbols "On"
        optimize "On"
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/glfw/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake ..",
            "cmake --build . --config Debug"
        }
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Fusion")
        }

    filter "configurations:Release"
        libdirs
        {
            "%{prj.location}/third_party/glfw/build/src/Release/"
        }
        optimize "On"
        symbols "On"
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/glfw/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake ..",
            "cmake --build . --config Debug"
        }
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Fusion")
        }
     filter "configurations:Development"
        libdirs
        {
            "%{prj.location}/third_party/glfw/build/src/Release/"
        }
        symbols "On"
        optimize "On"
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/glfw/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake ..",
            "cmake --build . --config Release"
        }
        outputDirImGui = "Development_%{cfg.system}_%{cfg.architecture}"
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDirImGui .. "/Fusion")
        }

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
        "%{prj.location}/third_party/flecs/distr/flecs.c"
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
        "%{prj.location}/third_party/glfw/include",
        "%{prj.location}/third_party/flecs/include/"
    }
    
    
    links
    {
        "raylib",
    }

    vectorextensions "Default"

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "off"
        systemversion "latest"
        buildoptions "/MP /nologo /W3 /wd4251 /wd4996 /wd4005 /wd4002"
        defines
        {
            "VE_WINDOWS",
            "VE_EXPORT",
            "USE_LIBTYPE_SHARED",
            "BUILD_LIBTYPE_SHARED",
            "flecs_EXPORTS"
        }

        libdirs 
        {
            "%{wks.location}/bin/" .. outputDir .. "/ImGui/"
            
        }

        links
        { 
            "ImGui.lib"
        }
    
    filter "configurations:Debug"
        libdirs
        {
            "%{prj.location}/third_party/raylib/build/raylib/Debug/",
        }
        defines "VE_DEBUG"
        symbols "On"
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/raylib/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF -DBUILD_GAMES=OFF -DBUILD_TESTING=OFF ..",
            "cmake --build . --config Debug",

            "{CHDIR} %{prj.location}",
            "%{wks.location}/bin/" .. outputDir .. "/VEHeaderTool/VEHeaderTool.exe e  src/generated/ src/components/ -c VE_COMPONENT -e VE_ENUM -f VE_SYSTEM -p VE_PROPERTY"
        }
        postbuildcommands
        {
            "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Fusion",
            "{COPY} \"%{wks.location}/VEngine/third_party/raylib/build/raylib/Debug/raylib.dll\" ../bin/" .. outputDir .. "/Fusion"
        }

    filter "configurations:Release"
        libdirs
        {
            "%{prj.location}/third_party/raylib/build/raylib/Release/",
        }
        defines "VE_RELEASE"
        optimize "On"
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/raylib/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF -DBUILD_GAMES=OFF -DBUILD_TESTING=OFF ..",
            "cmake --build . --config Release"
        }
        postbuildcommands
        {
            "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Fusion",
            "{COPY} \"%{wks.location}/VEngine/third_party/raylib/build/raylib/Release/raylib.dll\" ../bin/" .. outputDir .. "/Fusion"
        }


    filter "configurations:Development"
        libdirs
        {
            "%{prj.location}/third_party/raylib/build/raylib/Release/",
        }
        defines "VE_DEBUG"
        symbols "On"
        optimize "On" 
        prebuildcommands
        {
            "{CHDIR} %{prj.location}/third_party/raylib/",
            "{MKDIR} build",
            "{CHDIR} build",
            "cmake -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF -DBUILD_GAMES=OFF -DBUILD_TESTING=OFF ..",
            "cmake --build . --config Release"
        }
        outputDirVEngine = "Development_%{cfg.system}_%{cfg.architecture}"
        postbuildcommands
        {
            "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDirVEngine .. "/Fusion",
            "{COPY} \"%{wks.location}/VEngine/third_party/raylib/build/raylib/Release/raylib.dll\" ../bin/" .. outputDirVEngine .. "/Fusion"
        }
