outputDir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

--windows build flags win32.
windows_build_options = {
    "/utf-8",
    "/MP",
    "/nologo",
    "/W3",
    "/wd4251",
    "/wd4996",
    "/wd4005",
    "/wd4002"
}
--windows defines
windows_defines = {"VE_WINDOWS", "_GLFW_WIN32", "PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_43"}
windows_link_libs = {"Winmm.lib"}
--base defines.
base_engine_defines = 
{
    "FT2_BUILD_LIBRARY"
}
--editor defines
editor_engine_defines = 
{   "VE_EDITOR",
    "VE_EXPORT", 
    "BUILD_LIBTYPE_SHARED", 
    "flecs_EXPORTS",
    "IMGUI_EXPORT",
    "RLGL_ENABLE_OPENGL_DEBUG_CONTEXT",
}
--VE_Engine debug defines.
debug_engine_defines = 
{
    "VE_DEBUG"
}
--Editor debug defines
debug_editor_defines = {}
--VE_Engine release  defines.
release_engine_defines = {"VE_RELEASE"}
--Editor release defines
release_editor_defines = {}


base_dist_engine_defines = 
{
    base_engine_defines,
    "flecs_STATIC"
}

debug_dist_engine_defines = 
{
    debug_engine_defines,
}

release_dist_engine_defines = 
{
    release_engine_defines,
}

--utfcpp include dirs
utfcpp_include_dirs = {"VEngine/third_party/utfcpp/source/"}
--raylib include dirs
raylib_include_dirs = {"VEngine/third_party/raylib/src/"}
raylib_external_include_dirs = {"VEngine/third_party/raylib/src/external/"}
--glm include dirs.
glm_include_dirs = {"VEngine/third_party/glm/"}
--freetype include files.
freetype_include_dirs = {"VEngine/third_party/freetype/include/"}
--imgui include dirs.
imgui_include_dirs = {"VEngine/third_party/imgui/"}
--rlimgui include dirs.
rlimgui_include_dirs = {"VEngine/third_party/rlImGui/"}
--imguizmos include dirs.
imguizmos_include_dirs = {"VEngine/third_party/ImGuizmo/"}
--nlohmann include dirs.
nlohmann_include_dirs = {"VEngine/third_party/nlohmann_json/include/"}
--glfw include dirs
glfw_include_dirs = {"VEngine/third_party/glfw/include"}
--flecs include dirs
flecs_include_dirs = {"VEngine/third_party/flecs/include/"}
--shaping engine include dirs
shaping_engine_include_dirs = {"VEngine/third_party/ShapingEngine/"}
--AssetsPackager include dirs
assets_packager_include_dir = {"VEAssetsPackager/src/"}
--Base VE_Engine include dirs.
base_engine_include_dirs = 
{
    "VEngine/src/",
    raylib_include_dirs,
    glm_include_dirs,
    freetype_include_dirs,
    nlohmann_include_dirs,
    glfw_include_dirs,
    flecs_include_dirs,
    shaping_engine_include_dirs,
    utfcpp_include_dirs,
    raylib_external_include_dirs,
}
--Editor VE_Engine include dirs.
editor_engine_include_dirs = 
{
    imgui_include_dirs,
    rlimgui_include_dirs,
    imguizmos_include_dirs
}



--raylib src files
raylib_src_files = 
{
    "VEngine/third_party/raylib/src/raudio.c",
    "VEngine/third_party/raylib/src/rcore.c",
    "VEngine/third_party/raylib/src/rmodels.c",
    "VEngine/third_party/raylib/src/rshapes.c",
    "VEngine/third_party/raylib/src/rtext.c",
    "VEngine/third_party/raylib/src/rtextures.c",
    "VEngine/third_party/raylib/src/utils.c",
}
--imgui src files. add rlimgui to it.
imgui_src_files = 
{
    "VEngine/third_party/imgui/imgui.cpp",
    "VEngine/third_party/imgui/imgui_draw.cpp",
    "VEngine/third_party/imgui/imgui_tables.cpp",
    "VEngine/third_party/imgui/imgui_widgets.cpp",
    "VEngine/third_party/imgui/imgui_demo.cpp",
    "VEngine/third_party/imgui/backends/imgui_impl_opengl3.cpp",
    "VEngine/third_party/ImGuizmo/ImGuizmo.cpp",
}

rlimgui_src_files = 
{
    "VEngine/third_party/rlImGui/rlImGui.cpp"
}

--flecs src files.
flecs_src_files = 
{
    "VEngine/third_party/flecs/distr/flecs.c"
}
--glfw src dirs
glfw_src_files = 
{
    "VEngine/third_party/glfw/src/context.c",
    "VEngine/third_party/glfw/src/init.c",
    "VEngine/third_party/glfw/src/input.c",
    "VEngine/third_party/glfw/src/monitor.c",
    "VEngine/third_party/glfw/src/platform.c",
    "VEngine/third_party/glfw/src/vulkan.c",
    "VEngine/third_party/glfw/src/window.c",
    "VEngine/third_party/glfw/src/egl_context.c",
    "VEngine/third_party/glfw/src/osmesa_context.c",
    "VEngine/third_party/glfw/src/null_init.c",
    "VEngine/third_party/glfw/src/null_monitor.c",
    "VEngine/third_party/glfw/src/null_window.c",
    "VEngine/third_party/glfw/src/null_joystick.c",
    "VEngine/third_party/glfw/src/win32_module.c",
    "VEngine/third_party/glfw/src/win32_time.c",
    "VEngine/third_party/glfw/src/win32_thread.c",
    "VEngine/third_party/glfw/src/win32_init.c",
    "VEngine/third_party/glfw/src/win32_joystick.c",
    "VEngine/third_party/glfw/src/win32_monitor.c",
    "VEngine/third_party/glfw/src/win32_window.c",
    "VEngine/third_party/glfw/src/wgl_context.c",
}
--freetype src files.
freetype_src_files = 
{
    "VEngine/third_party/freetype/src/autofit/autofit.c",
    "VEngine/third_party/freetype/src/base/ftbase.c",
    "VEngine/third_party/freetype/src/base/ftbbox.c",
    "VEngine/third_party/freetype/src/base/ftbdf.c",
    "VEngine/third_party/freetype/src/base/ftbitmap.c",
    "VEngine/third_party/freetype/src/base/ftcid.c",
    "VEngine/third_party/freetype/src/base/ftfstype.c",
    "VEngine/third_party/freetype/src/base/ftgasp.c",
    "VEngine/third_party/freetype/src/base/ftglyph.c",
    "VEngine/third_party/freetype/src/base/ftgxval.c",
    "VEngine/third_party/freetype/src/base/ftinit.c",
    "VEngine/third_party/freetype/src/base/ftmm.c",
    "VEngine/third_party/freetype/src/base/ftotval.c",
    "VEngine/third_party/freetype/src/base/ftpatent.c",
    "VEngine/third_party/freetype/src/base/ftpfr.c",
    "VEngine/third_party/freetype/src/base/ftstroke.c",
    "VEngine/third_party/freetype/src/base/ftsynth.c",
    "VEngine/third_party/freetype/src/base/fttype1.c",
    "VEngine/third_party/freetype/src/base/ftwinfnt.c",
    "VEngine/third_party/freetype/src/base/ftsystem.c",
    "VEngine/third_party/freetype/src/base/ftdebug.c",
    "VEngine/third_party/freetype/src/bdf/bdf.c",
    "VEngine/third_party/freetype/src/bzip2/ftbzip2.c",
    "VEngine/third_party/freetype/src/cache/ftcache.c",
    "VEngine/third_party/freetype/src/cff/cff.c",
    "VEngine/third_party/freetype/src/cid/type1cid.c",
    "VEngine/third_party/freetype/src/gzip/ftgzip.c",
    "VEngine/third_party/freetype/src/lzw/ftlzw.c",
    "VEngine/third_party/freetype/src/pcf/pcf.c",
    "VEngine/third_party/freetype/src/pfr/pfr.c",
    "VEngine/third_party/freetype/src/psaux/psaux.c",
    "VEngine/third_party/freetype/src/pshinter/pshinter.c",
    "VEngine/third_party/freetype/src/psnames/psnames.c",
    "VEngine/third_party/freetype/src/raster/raster.c",
    "VEngine/third_party/freetype/src/sdf/sdf.c",
    "VEngine/third_party/freetype/src/sfnt/sfnt.c",
    "VEngine/third_party/freetype/src/smooth/smooth.c",
    "VEngine/third_party/freetype/src/svg/svg.c",
    "VEngine/third_party/freetype/src/truetype/truetype.c",
    "VEngine/third_party/freetype/src/type1/type1.c",
    "VEngine/third_party/freetype/src/type42/type42.c",
    "VEngine/third_party/freetype/src/winfonts/winfnt.c",
}
--Base VE_Engine source files.
base_engine_src_files = 
{
    "VEngine/src/**.h",     
    "VEngine/src/**.hpp",      
    "VEngine/src/**.cpp",      
    "VEngine/src/**.c",
    raylib_src_files,
    flecs_src_files,
    freetype_src_files,
}
--Editor VE_Engine source files.
editor_engine_src_files = 
{
    imgui_src_files,
    rlimgui_src_files,
}

--helper functions 
function prefixPaths(prefix, paths)
    local newPaths = {}
    for _, p in ipairs(paths) do
        -- If the element is itself a table (like nested source lists), 
        -- apply recursively
        if type(p) == "table" then
            table.insert(newPaths, prefixPaths(prefix, p))
        else
            table.insert(newPaths, prefix .. p)
        end
    end
    return newPaths
end