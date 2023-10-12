#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#include "vulkan/tge_device.hpp"

namespace tge {
  class TgeEditorUI {
    public:

      TgeEditorUI();
      ~TgeEditorUI();

      TgeEditorUI(const TgeEditorUI &) = delete;
			TgeEditorUI &operator=(const TgeEditorUI &) = delete;

    private:
  };
}