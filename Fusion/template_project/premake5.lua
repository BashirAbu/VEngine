include (os.getenv("VENGINE_DIR") .. "/premake/premake-emscripten/emscripten.lua")
include (os.getenv("VENGINE_DIR") .. "/premake/premake-cmake/cmake.lua")

local ve_engine_path = os.getenv("VENGINE_DIR")
local build_globals = path.join(ve_engine_path, "build_globals.lua")
local targetName = "SpaceShooter"
include(build_globals)

workspace "Project"
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
        architecture "x64"
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
        files
        {
            "resource.rc",
            prefixPaths(os.getenv("VENGINE_DIR") .. "/", glfw_src_files),
        }
    filter {"configurations:Debug", "system:windows"}
        local currentTime = os.date("%Y%m%d%H%M%S")
        linkoptions 
        {
            "/PDB:bin\\%{prj.name}_" .. currentTime .. ".pdb"
        }
    filter {"configurations:Game_Debug or configurations:Game_Release", "system:windows"}
    postbuildcommands
        {
            "{COPY} icon.png %{cfg.buildtarget.directory}",
        }
    filter {"configurations:Game_Debug or configurations:Game_Release", "system:windows"}
        prebuildcommands
        {
            "{CHDIR} %{cfg.buildtarget.directory}",
            os.getenv("VENGINE_DIR") .. "/bin/Release_windows_x86_64/VEAssetsPackager/VEAssetsPackager %{prj.location}",
        }
    filter "system:emscripten"
        architecture "wasm32" 
        system "emscripten"
        buildoptions 
        {   
            "-sUSE_PTHREADS=1",
            "-msimd128",
        }
        linkoptions
        {
            "-o %{cfg.buildtarget.directory}/" .. targetName .. ".html",
            "-sWASM=1",
            "-sUSE_GLFW=3",
            "--preload-file %{cfg.buildtarget.directory}/data.VEData@./data.VEData",
            "-s STACK_SIZE=4194304",
            "-sALLOW_MEMORY_GROWTH"
        }
        defines
        {
            "GRAPHICS_API_OPENGL_ES2",
            "VE_WEB",
            "PLATFORM_WEB",
        }
    filter {"files:*.c", "system:emscripten"}
        buildoptions 
        {  
            "--std:c++20"
        }

        
    
    filter "configurations:Debug or configurations:Release"

        prebuildcommands
        {
            "{DELETE} %{prj.location}/bin/*.pdb",
        }
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
        
        targetname (targetName)
        debugdir ("%{cfg.buildtarget.directory}")
        

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
            prefixPaths(os.getenv("VENGINE_DIR") .. "/", base_engine_include_dirs),
            prefixPaths(os.getenv("VENGINE_DIR") .. "/", assets_packager_include_dir), 
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