#include "LayerStack.h"
#include "mzpch.h"

namespace Mochii {
LayerStack::LayerStack() {}

LayerStack::~LayerStack() {
  for (Layer* layer : layers_) {
    layer->OnDetach();
    delete layer;
  }
}

void LayerStack::PushLayer(Layer* layer) {
  MI_CORE_ASSERT(layer, "PushLayer called with null pointer!");
  layers_.emplace(layers_.begin() + layerInsertIndex_, layer);
  layerInsertIndex_++;
  layer->OnAttach();
}

void LayerStack::PushOverlay(Layer* overlay) {
  MI_CORE_ASSERT(overlay, "PushOverlay called with null pointer!");
  layers_.emplace_back(overlay);
  overlay->OnAttach();
}

void LayerStack::PopLayer(Layer* layer) {
  auto it =
      std::find(layers_.begin(), layers_.begin() + layerInsertIndex_, layer);
  if (it != layers_.begin() + layerInsertIndex_) {
    layer->OnDetach();
    layers_.erase(it);
    layerInsertIndex_--;
    delete layer;
  }
}

void LayerStack::PopOverlay(Layer* overlay) {
  auto it =
      std::find(layers_.begin() + layerInsertIndex_, layers_.end(), overlay);
  if (it != layers_.end()) {
    overlay->OnDetach();
    layers_.erase(it);
    delete overlay;
  }
}
}  // namespace Mochii
