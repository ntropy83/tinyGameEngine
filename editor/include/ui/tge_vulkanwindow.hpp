#pragma once

#include "vulkan/systems/simple_render_system.hpp"
#include "vulkan/tge_device.hpp"
#include "vulkan/tge_window.hpp"
#include "tge_game_object.hpp"
#include "vulkan/tge_renderer.hpp"

// std
#include <memory>
#include <vector>

namespace tge {

  class TgeEditor {

    public:

      static constexpr int WIDTH = 1280;
      static constexpr int HEIGHT = 800;
      
      TgeEditor();
      ~TgeEditor();

      TgeEditor(const TgeEditor &) = delete;
			TgeEditor &operator=(const TgeEditor &) = delete;

      TgeWindow& getWindow() { return tgeWindow; }
      
      void update();

    private:

      void loadGameObjects(); 

      TgeWindow tgeWindow;
      TgeDevice tgeDevice;
      TgeRenderer tgeRenderer{tgeWindow, tgeDevice};
      std::vector<TgeGameObject> gameObjects;
      TgeCamera camera;

      SimpleRenderSystem simpleRenderSystem;

      bool window_should_close(){ return tgeWindow.shouldClose(); }
  };
} // namespace tge