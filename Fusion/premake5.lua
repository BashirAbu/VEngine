project "Fusion"
    cppdialect "C++20"
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
        "src/",
        prefixPaths("../", base_engine_include_dirs),
        prefixPaths("../", editor_engine_include_dirs)
    }
    
    libdirs 
    {
        "%{wks.location}/bin/" .. outputDir .. "/VEngine/"   
    }

    links
    {
        "VEngine.lib"
    }
    
    filter "system:windows"
        
        systemversion "latest"
        buildoptions { windows_build_options }
        defines { windows_defines }

    
    filter "configurations:Debug"
        staticruntime "off"
        defines 
        {   "VE_DEBUG",
            "VE_EDITOR",
            "USE_LIBTYPE_SHARED"             
        }
        symbols "On"

    filter "configurations:Release"
        staticruntime "On"
        defines 
        {
            "VE_RELEASE",
            "VE_EDITOR",
            "USE_LIBTYPE_SHARED"             
        }
        optimize "On"
