local ve_engine_path = os.getenv("VENGINE_DIR")
local build_globals = path.join(ve_engine_path, "build_globals.lua")

include(build_globals)

workspace "Project"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Game_Debug",
        "Game_Release"
    }

project "project"
    
    language "c++"
    cppdialect "C++20"

    prebuildcommands
    {
        "{CHDIR} %{prj.location}",
        os.getenv("VENGINE_DIR") .. "/bin/Debug_windows_x86_64//VEHeaderTool/VEHeaderTool.exe p src/generated/ src/ -c VE_CLASS -e VE_ENUM -f VE_FUNCTION -p VE_PROPERTY"
    }

    filter "system:windows"
        systemversion "latest"
        
        buildoptions {windows_build_options}
        links
        {
            windows_link_libs
        }
        defines
        {
            windows_defines
        }

        
    filter "configurations:Debug or configurations:Release"
        defines 
        {
            "VE_PROJECT_EXPORT",
            "USE_LIBTYPE_SHARED",
            "VE_EDITOR"
        }
        debugargs
        {
            "-p %{prj.location}"
        }
        debugdir (os.getenv("VENGINE_DIR") .. "/Fusion")

        debugcommand (os.getenv("VENGINE_DIR") .. "/bin/" .. outputDir .. "/Fusion/Fusion")
        
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
            prefixPaths(os.getenv("VENGINE_DIR") .. "/", base_engine_include_dirs),
            prefixPaths(os.getenv("VENGINE_DIR") .. "/", editor_engine_include_dirs)
        }
        libdirs 
        {
            os.getenv("VENGINE_DIR") .. "/bin/" .. outputDir .. "/VEngine/"
        }

        links
        {
            "VEngine"
        }
        kind "SharedLib"
        staticruntime "Off"
        targetdir("bin/")
        objdir("bin/bin_int")


    filter "configurations:Debug"
        symbols "On"
        local currentTime = os.date("%Y%m%d%H%M%S")
        linkoptions 
        {
            "/PDB:bin\\%{prj.name}_" .. currentTime .. ".pdb"
        }
        defines 
        {
            "VE_DEBUG",
        }
    
    filter "configurations:Release"
        optimize "On"
        defines 
        {
            "VE_RELEASE",
        }
    filter "configurations:Game_Debug or configurations:Game_Release"
        files
        {
            "%{prj.location}/src/**.h",     
            "%{prj.location}/src/**.hpp",      
            "%{prj.location}/src/**.cpp",      
            "%{prj.location}/src/**.c",
            prefixPaths(os.getenv("VENGINE_DIR") .. "/", base_engine_src_files),
        }

        includedirs
        {
            "src/",
            prefixPaths(os.getenv("VENGINE_DIR") .. "/", base_engine_include_dirs)
        }

        kind "ConsoleApp"

        defines 
        {   
            base_dist_engine_defines
        }
        targetdir("dist/" .. outputDir .. "/bin")
        objdir("dist/" .. outputDir .. "/bin_int/")

    filter "configurations:Game_Debug"
        defines 
        {
            debug_dist_engine_defines
        }
        staticruntime "Off"
        symbols "On"
        
    filter "configurations:Game_Release"
        defines 
        {
            release_dist_engine_defines
        }
        staticruntime "On"
        optimize "On"