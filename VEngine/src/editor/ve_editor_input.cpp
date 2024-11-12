#include "ve_editor_input.h"
#include <raylib.h>
#include "ve_editor.h"

namespace VE
{
    namespace EditorInput
    {
        bool IsKeyPressed(int key)
        {
            if (!Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyPressed(key);
            }
            return false;
        }
        bool IsKeyPressedRepeat(int key)
        {
            if (!Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyPressedRepeat(key);
            }
            return false;
        }
        bool IsKeyDown(int key)
        {
            if (!Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyDown(key);
            }
            return false;
        }
        bool IsKeyReleased(int key)
        {
            if (!Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyReleased(key);
            }
            return false;
        }
        bool IsKeyUp(int key)
        {
            if (!Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyUp(key);
            }
            return false;
        }
        int GetKeyPressed(void)
        {
            if (!Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::GetKeyPressed();
            }
            return KEY_NULL;
        }
        int GetCharPressed(void)
        {
            if (!Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::GetCharPressed();
            }
            return KEY_NULL;
        }
        glm::vec2 GetMousePostion()
        {
            //float scale = glm::min((float)GetScreenWidth() / gameScreenWidth, (float)GetScreenHeight() / gameScreenHeight;
            Vector2 mousePos = GetMousePosition();
            glm::vec2 virMousePos = {};
            virMousePos.x = mousePos.x - (Editor::GetSingleton()->GetSceneViewportPosition()).x;
            virMousePos.y = mousePos.y - (Editor::GetSingleton()->GetSceneViewportPosition()).y;
            virMousePos = glm::clamp(virMousePos, glm::vec2(0.0f, 0.0f), Editor::GetSingleton()->GetSceneViewportSize());
            return virMousePos;
        }
    }
}