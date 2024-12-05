#include "ve_utils.h"
#include "components/ve_components.h"
#include "ve_font.h"
#include <hb.h>
#include <hb-ft.h>
#include <codecvt>
#include <locale>
#include "ShapingEngine.hpp"
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


    VE_API void RaylibDrawTextUTF8(const Renderer::Label2D& label)
    {
        int32_t x = 0;
        for (wchar_t c : label.wideString)
        {
           
            if (c == '\0') { return; }

            const Glyph& glyph = label.font->GetGlypth(c);
            Rectangle src = {};
                
            src.x = 0.0f;
            src.y = 0.0f;
            src.width = (float)glyph.texture.width;
            src.height = (float)glyph.texture.height;

            Rectangle dest = {};
            glm::mat4 matrix = glm::translate(label.worldTransformMatrix, glm::vec3(x + glyph.bearingX,- glyph.bearingY, 0.0f));
            matrix = glm::scale(matrix, glm::vec3((label.fontSize / label.font->GetFontSize()), (label.fontSize / label.font->GetFontSize()), 1.0f));
            glm::vec3 pos;
            glm::vec3 scl;
            glm::quat rot;
            glm::vec4 presp;
            glm::vec3 skew;

            glm::decompose(matrix, scl, rot, pos, skew, presp);

            glm::vec3 rotation = glm::degrees(glm::eulerAngles(rot));

            dest.x = pos.x;
            dest.y = pos.y;

            dest.width = glyph.texture.width * scl.x;
            dest.height = glyph.texture.height * scl.y;

            DrawTexturePro(glyph.texture, src, dest, *(Vector2*) & label.origin, label.rotation, label.tint);

            x += (glyph.bearingX + (int)(glyph.advance * label.spacing));
        }
    }
}
