#pragma once
#include <vector>
#include "Mochii/Core/Core.h"
#include "Mochii/Core/Layer.h"

namespace Mochii {
class LayerStack {
 public:
  LayerStack() = default;
  ~LayerStack();

  void PushLayer(Layer* layer);
  void PushOverlay(Layer* overlay);
  void PopLayer(Layer* layer);
  void PopOverlay(Layer* overlay);

  std::vector<Layer*>::iterator begin() { return layers_.begin(); }
  std::vector<Layer*>::iterator end() { return layers_.end(); }

 private:
  std::vector<Layer*> layers_;
  std::size_t layerInsertIndex_ = 0;
};
}  // namespace Mochii
