#include "ve_input.h"
#include <raylib.h>
#include "ve_engine.h"
#include "components/ve_components.h"
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
        VE_API bool IsMouseButtonPressed(int button)
        {
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsMouseButtonPressed(button);
            }
            return false;
        }

        VE_API bool IsMouseButtonDown(int button)
        {
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsMouseButtonDown(button);
            }
            return false;
        }
        VE_API bool IsMouseButtonReleased(int button)
        {
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsMouseButtonReleased(button);
            }
            return false;
        }
        VE_API bool IsMouseButtonUp(int button)
        {
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsMouseButtonUp(button);
            }
            return false;
        }
        VE_API glm::vec2 GetMousePosistion()
        {
            glm::vec2 pos = {};
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                glm::vec2 gameViewportPos = Editor::GetSingleton()->GetGameViewportPosition();
                pos.x = ::GetMousePosition().x - gameViewportPos.x;
                pos.y = ::GetMousePosition().y - gameViewportPos.y;
                glm::vec2 gameViewportSize = Editor::GetSingleton()->GetGameViewportSize();
                pos.x /= gameViewportSize.x;
                pos.y /= gameViewportSize.y;

                
                pos.x *= VE::Scene::GetSingleton()->renderer.GetMainRenderTarget().texture.width;
                pos.y *= VE::Scene::GetSingleton()->renderer.GetMainRenderTarget().texture.height;
                

            }
            return pos;
        }
    }
}