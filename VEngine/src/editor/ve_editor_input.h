#pragma once
#include "ve_defines.h"
#include "glm/glm.hpp"
namespace VE
{
    namespace EditorInput
    {
        VE_API bool IsKeyPressed(int key);
        VE_API bool IsKeyPressedRepeat(int key);
        VE_API bool IsKeyDown(int key);
        VE_API bool IsKeyReleased(int key);
        VE_API bool IsKeyUp(int key);
        VE_API int GetKeyPressed(void);
        VE_API int GetCharPressed(void);
        VE_API glm::vec2 GetMousePostion();
    }
}