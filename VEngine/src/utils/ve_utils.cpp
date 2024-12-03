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
    VE_API void RaylibDrawTexturTargeteLetterBox(RenderTexture renderTarget, glm::vec2 screenSize)
    {
        float scale = glm::min(screenSize.x / renderTarget.texture.width, screenSize.y / renderTarget.texture.height);


        DrawTexturePro(renderTarget.texture, {0.0f, 0.0f, (float) renderTarget.texture.width, (float) -renderTarget.texture.height},
            {(screenSize.x - (renderTarget.texture.width * scale)) * .5f, (screenSize.y - (renderTarget.texture.height * scale)) * .5f, renderTarget.texture.width * scale, renderTarget.texture.height * scale }
        , {0.0f, 0.0f}, 0.0f, WHITE);
    }
}
