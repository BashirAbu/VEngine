#pragma once
#include "ve_defines.h"
#include <glm/glm.hpp>
namespace VE 
{
	namespace Input 
	{
        VE_API bool IsKeyPressed(int key);                             
        VE_API bool IsKeyPressedRepeat(int key);                       
        VE_API bool IsKeyDown(int key);                                
        VE_API bool IsKeyReleased(int key);                            
        VE_API bool IsKeyUp(int key);                                  
        VE_API int GetKeyPressed(void);                                
        VE_API int GetCharPressed(void);
        VE_API bool IsMouseButtonPressed(int button);
        VE_API bool IsMouseButtonDown(int button);
        VE_API bool IsMouseButtonReleased(int button);
        VE_API bool IsMouseButtonUp(int button);
        VE_API glm::vec2 GetMousePosistion();
	}
}