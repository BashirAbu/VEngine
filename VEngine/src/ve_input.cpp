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
#ifdef VE_EDITOR
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyPressed(key);
            }
            return false;
#else
            return ::IsKeyPressed(key);
#endif


        }
        bool IsKeyPressedRepeat(int key) 
        {
#ifdef VE_EDITOR
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyPressedRepeat(key);
            }
            return false;
#else
            return ::IsKeyPressedRepeat(key);
#endif
        }
        bool IsKeyDown(int key) 
        {
#ifdef VE_EDITOR
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyDown(key);
            }
            return false;
#else
            return ::IsKeyDown(key);
#endif
        }
        bool IsKeyReleased(int key) 
        {
#ifdef VE_EDITOR
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyReleased(key);
            }
            return false;
#else
            return ::IsKeyReleased(key);
#endif
        }
        bool IsKeyUp(int key) 
        {
#ifdef VE_EDITOR
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsKeyUp(key);
            }
            return false;
#else
            return ::IsKeyUp(key);
#endif
        }
        int GetKeyPressed(void) 
        {

#ifdef VE_EDITOR
            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::GetKeyPressed();
            }
            return KEY_NULL;
#else
            return ::GetKeyPressed();
#endif
        }
        int GetCharPressed(void) 
        {
#ifdef VE_EDITOR

            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::GetCharPressed();
            }
            return KEY_NULL;
#else
            return ::GetCharPressed();
#endif

        }
        VE_API bool IsMouseButtonPressed(int button)
        {
#ifdef VE_EDITOR

            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsMouseButtonPressed(button);
            }
            return false;
#else
            return ::IsMouseButtonPressed(button);
#endif
        }

        VE_API bool IsMouseButtonDown(int button)
        {
#ifdef VE_EDITOR

            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsMouseButtonDown(button);
            }
            return false;
#else
            return ::IsMouseButtonDown(button);
#endif
        }
        VE_API bool IsMouseButtonReleased(int button)
        {
#ifdef VE_EDITOR

            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsMouseButtonReleased(button);
            }
            return false;
#else
            return ::IsMouseButtonReleased(button);
#endif
        }
        VE_API bool IsMouseButtonUp(int button)
        {
#ifdef VE_EDITOR

            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                return ::IsMouseButtonUp(button);
            }
            return false;
#else
            return ::IsMouseButtonUp(button);
#endif
        }
        VE_API glm::vec2 GetMousePosistion()
        {
            glm::vec2 pos = {};

#ifdef VE_EDITOR

            if (Editor::GetSingleton()->IsGameViewportFocused())
            {
                glm::vec2 gameViewportPos = Editor::GetSingleton()->GetGameViewportPosition();
                pos.x = ::GetMousePosition().x - gameViewportPos.x;
                pos.y = ::GetMousePosition().y - gameViewportPos.y;
                glm::vec2 gameViewportSize = Editor::GetSingleton()->GetGameViewportSize();
                pos.x /= gameViewportSize.x;
                pos.y /= gameViewportSize.y;

                
                pos.x *= VE::Scene::GetSingleton()->GetMainRenderTarget().texture.width;
                pos.y *= VE::Scene::GetSingleton()->GetMainRenderTarget().texture.height;
                

            }
#else
            //get corner pos.
            glm::vec2 screenSize = { (float)GetScreenWidth(), (float)GetScreenHeight()};
            RenderTexture renderTarget = VE::Scene::GetSingleton()->GetMainRenderTarget();
            float scale = glm::min(screenSize.x / renderTarget.texture.width, screenSize.y / renderTarget.texture.height);

            Rectangle drawAeraRect = { (screenSize.x - (renderTarget.texture.width * scale)) * .5f, (screenSize.y - (renderTarget.texture.height * scale)) * .5f,
                renderTarget.texture.width * scale, renderTarget.texture.height * scale };

            pos.x = ::GetMousePosition().x - drawAeraRect.x;
            pos.y = ::GetMousePosition().y - drawAeraRect.y;

            pos.x /= drawAeraRect.width;
            pos.y /= drawAeraRect.height;

            pos.x *= renderTarget.texture.width;
            pos.y *= renderTarget.texture.height;
#endif
            return pos;
        }
    }
}