project "VEHeaderTool"
    kind "ConsoleApp"
    language "c++"
    cppdialect "C++20"
    staticruntime "off"

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
        systemversion "latest"
        buildoptions "/MP /nologo /W3 /wd4251 /wd4996 /wd4005"
    
    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"
