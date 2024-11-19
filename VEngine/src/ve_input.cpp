#include "ve_input.h"
#include <raylib.h>
#include "ve_engine.h"
namespace VE 
{
    namespace Input 
    {
        bool IsKeyPressed(int key) 
        {
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyPressed(key);
            }
            return false;
        }
        bool IsKeyPressedRepeat(int key) 
        {
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyPressedRepeat(key);
            }
            return false;
        }
        bool IsKeyDown(int key) 
        {
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyDown(key);
            }
            return false;
        }
        bool IsKeyReleased(int key) 
        {
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyReleased(key);
            }

            return false;
        }
        bool IsKeyUp(int key) 
        {
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyUp(key);
            }
            return false;
        }
        int GetKeyPressed(void) 
        {
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::GetKeyPressed();
            }
            return KEY_NULL;
        }
        int GetCharPressed(void) 
        {
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::GetCharPressed();
            }
            return KEY_NULL;
        }
        VE_API glm::vec2 GetMousePosistion()
        {
            glm::vec2 pos = {};
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                glm::vec2 gameViewportPos = Editor::GetSingleton()->GetGameViewportPosition();
                pos.x = ::GetMousePosition().x - gameViewportPos.x;
                pos.y = ::GetMousePosition().y - gameViewportPos.y;
            }
            return pos;
        }
    }
}