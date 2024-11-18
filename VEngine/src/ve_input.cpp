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
    }
}