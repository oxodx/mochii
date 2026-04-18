#pragma once
#include <vector>
#include "Layer.h"
#include "Mochii/Core/Core.h"

namespace Mochii {
class MOCHII_API LayerStack {
 public:
  LayerStack();
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
