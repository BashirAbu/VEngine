#include "ve_utils.h"
#include "components/ve_components.h"
namespace VE 
{

    Color GLMVec4ToRayColor(glm::vec4 color)
    {
        return Color{ uint8_t(color.r * 255.0f), uint8_t(color.g * 255.0f), uint8_t(color.b * 255.0f), uint8_t(color.a * 255.0f) };
    }
    
    std::filesystem::path GetRelativePath(std::filesystem::path path)
    {
        return path.lexically_relative(Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().generic_string() + "/assets");
    }
    std::filesystem::path GetFullPath(std::filesystem::path path)
    {
        return Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().string() + "/assets/" + path.string();
    }
}
