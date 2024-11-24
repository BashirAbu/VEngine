#include "ve_utils.h"
namespace VE 
{

    Color GLMVec4ToRayColor(glm::vec4 color)
    {
        return Color{ uint8_t(color.r * 255.0f), uint8_t(color.g * 255.0f), uint8_t(color.b * 255.0f), uint8_t(color.a * 255.0f) };
    }
    glm::mat4 GetLocalTransformMatrix(const Components::TransformComponent& tc)
    {
        glm::mat4 transformMatrix = glm::mat4(1.0f);
        transformMatrix = glm::translate(glm::mat4(1.0f), tc.localPosition)
            * glm::rotate(glm::mat4(1.0f), glm::radians(tc.localRotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(tc.localRotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(tc.localRotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::scale(glm::mat4(1.0f), tc.localScale);
        return transformMatrix;
    }
    glm::mat4 GetWorldTransformMatrix(const Components::TransformComponent& tc)
    {
        glm::mat4 transformMatrix = glm::mat4(1.0f);
        transformMatrix = glm::translate(glm::mat4(1.0f), tc.worldPosition)
            * glm::rotate(glm::mat4(1.0f), glm::radians(tc.worldRotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(tc.worldRotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(tc.worldRotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::scale(glm::mat4(1.0f), tc.worldScale);
        return transformMatrix;
    }
}
