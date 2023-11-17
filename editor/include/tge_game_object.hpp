#pragma once

#include "vulkan/tge_model.hpp"

// std
#include <memory>

namespace tge {

    struct Transform2DComponent {
        glm::vec2 translation{}; // (position offset)
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 mat2() { 
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{{c,s},{-s,c}};

            glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
            return rotMatrix * scaleMat; 
        } // Identity Matrix
    };

    class TgeGameObject {
        public:
        using id_t = unsigned int;

        static TgeGameObject createGameObject() {
            static id_t currentId = 0;
            return TgeGameObject{currentId++};
        }

        TgeGameObject(const TgeGameObject &) = delete;
        TgeGameObject &operator=(const TgeGameObject &) = delete;
        TgeGameObject(TgeGameObject &&) = default;
        TgeGameObject &operator=(TgeGameObject &&) = default;        

        const id_t getId() { return id; }

        std::shared_ptr<TgeModel> model{};
        glm::vec3 color{};
        Transform2DComponent transform2D{};

        private:
        TgeGameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}