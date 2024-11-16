workspace "Project"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }
outputDir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"
project "project"
    kind "SharedLib"
    language "c++"

    targetdir("bin/")
    objdir("bin/bin_int")

    files
    {
        "%{prj.location}/src/**.h",     
        "%{prj.location}/src/**.hpp",      
        "%{prj.location}/src/**.cpp",      
        "%{prj.location}/src/**.c"    
    }

    includedirs
    {
        os.getenv("VENGINE_DIR") .. "/src",
        os.getenv("VENGINE_DIR") .. "/VEngine/src/",
        os.getenv("VENGINE_DIR") .. "/VEngine/third_party/raylib/build/raylib/include",
        os.getenv("VENGINE_DIR") .. "/VEngine/third_party/nlohmann_json/include/",
        os.getenv("VENGINE_DIR") .. "/VEngine/third_party/glm/",
        os.getenv("VENGINE_DIR") .. "/VEngine/third_party/imgui/",
        os.getenv("VENGINE_DIR") .. "/VEngine/third_party/rlImGui/",
    }
    
    libdirs
    {
        os.getenv("VENGINE_DIR") .. "/VEngine/third_party/raylib/build/raylib/%{cfg.buildcfg}/"
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
            "VE_PROJECT_EXPORT"
        }

        libdirs 
        {
            os.getenv("VENGINE_DIR") .. "/bin/" .. outputDir .. "/ImGui/",
            os.getenv("VENGINE_DIR") .. "/bin/" .. outputDir .. "/VEngine/"
            
        }

        links
        {
            "ImGui.lib",
            "VEngine.lib"
        }
    
    filter "configurations:Debug"
        defines "VE_DEBUG"
        symbols "On"
        local currentTime = os.date("%Y%m%d%H%M%S")
        linkoptions 
        {
            "/PDB:bin\\%{prj.name}_" .. currentTime .. ".pdb"
        }

    filter "configurations:Release"
        defines "VE_RELEASE"
        optimize "On"