#include "ve_utils.h"
#include "components/ve_components.h"
namespace VE 
{

    Color GLMVec4ToRayColor(glm::vec4 color)
    {
        return Color{ uint8_t(color.r * 255.0f), uint8_t(color.g * 255.0f), uint8_t(color.b * 255.0f), uint8_t(color.a * 255.0f) };
    }
    glm::mat4 GetLocalTransformMatrix(flecs::entity e)
    {
        Components::TransformComponent& tc = *e.get_mut<Components::TransformComponent>();
        glm::mat4 transformMatrix = glm::mat4(1.0f);
        transformMatrix = glm::translate(glm::mat4(1.0f), tc.localPosition)
            * glm::rotate(glm::mat4(1.0f), glm::radians(tc.localRotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(tc.localRotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(tc.localRotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::scale(glm::mat4(1.0f), tc.localScale);
        return transformMatrix;
    }
    glm::mat4 GetWorldTransformMatrix(flecs::entity e)
    {

        glm::mat4 transformMatrix = glm::mat4(1.0f);
        if (!e.parent())
        {
            transformMatrix = GetLocalTransformMatrix(e);
        }
        else 
        {
            glm::mat4 parentWorldTransformMatrix = GetWorldTransformMatrix(e.parent());

            glm::mat4 childLocalTransformMatrix = GetLocalTransformMatrix(e);

            transformMatrix = parentWorldTransformMatrix * childLocalTransformMatrix;
        }
        return transformMatrix;
    }
    std::filesystem::path GetRelativePath(std::filesystem::path path)
    {
        return path.lexically_relative(Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().generic_string() + "/assets");
    }
    std::filesystem::path GetFullPath(std::filesystem::path path)
    {
        return Engine::GetSingleton()->GetDesc()->projectDetails.path.parent_path().string() + "/assets/" + path.string();
    }
}
