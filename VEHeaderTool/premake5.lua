project "VEHeaderTool"
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
        "%{wks.location}/VEngine/third_party/nlohmann_json/include/"
    }
    
    
    vectorextensions "Default"
    
    filter "system:windows"
        cppdialect "C++20"
        staticruntime "off"
        systemversion "latest"
        buildoptions "/MP /nologo /W3 /wd4251 /wd4996 /wd4005"
        defines
        {
            "VE_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines 
        {
            "VE_DEBUG",
            "VE_EDITOR",
        }
        symbols "On"

    filter "configurations:Release"
        defines 
        {
            "VE_RELEASE",
            "VE_EDITOR",
        }
        optimize "On"

    filter "configurations:Game_Debug"

    defines "VE_DEBUG"
    symbols "On"

    filter "configurations:Game_Release"
    
        defines "VE_RELEASE"
        optimize "On"