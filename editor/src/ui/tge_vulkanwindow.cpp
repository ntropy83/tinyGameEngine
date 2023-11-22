#include "ui/tge_vulkanwindow.hpp"

#include "glm/gtc/constants.hpp"

#include "vulkan/systems/simple_render_system.hpp"

#include "vulkan/tge_window.hpp"

//std
#include <array>
#include <cassert>
#include <stdexcept>

namespace tge {

  TgeEditor::TgeEditor() :
      tgeWindow(WIDTH, HEIGHT, "Tge_3DView"),
      tgeDevice(tgeWindow),
      simpleRenderSystem(tgeDevice, tgeRenderer.getSwapChainRenderPass())
  {
    loadGameObjects();
  }

  TgeEditor::~TgeEditor() {}

  void TgeEditor::update() {
    
    glfwPollEvents();
    
    if (auto commandBuffer = tgeRenderer.beginFrame()) {
      tgeRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
      tgeRenderer.endSwapChainRenderPass(commandBuffer);
      tgeRenderer.endFrame();
    }
    
    vkDeviceWaitIdle(tgeDevice.device());
  }

  void TgeEditor::loadGameObjects() {
    std::vector<TgeModel::Vertex> vertices{
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
    
    auto tgeModel = std::make_shared<TgeModel>(tgeDevice, vertices);

    auto triangle = TgeGameObject::createGameObject();
    triangle.model = tgeModel;
    triangle.color = {.1f, .8f, .1f};
    triangle.transform2D.translation.x = .2f;
    triangle.transform2D.scale = {2.f, .5f};
    triangle.transform2D.rotation = .25f * glm::two_pi<float>();

    gameObjects.push_back(std::move(triangle));
  }
} // namespace tge