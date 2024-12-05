project "Fusion"
    kind "ConsoleApp"
    language "c++"

    targetdir("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir("%{wks.location}/bin_int/" .. outputDir .. "/%{prj.name}")

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
        "%{wks.location}/VEngine/third_party/flecs/include/",
        "%{wks.location}/VEngine/third_party/freetype/include/",
    }
    
    
    links
    {
        "raylib"
    }
    
    vectorextensions "Default"
    
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
        libdirs
        {
            "%{wks.location}/VEngine/third_party/raylib/build/raylib/Debug/"
        }
        defines "VE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        libdirs
        {
            "%{wks.location}/VEngine/third_party/raylib/build/raylib/Release/"
        }
        defines "VE_RELEASE"
        optimize "On"
    filter "configurations:Development"
        libdirs
        {
            "%{wks.location}/VEngine/third_party/raylib/build/raylib/Release/"
        }
        defines "VE_DEBUG"
        symbols "On"
        optimize "On" 