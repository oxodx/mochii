#include "Mochii/Core/LayerStack.h"
#include "mzpch.h"

namespace Mochii {
LayerStack::~LayerStack() {
  for (Layer* layer : m_Layers) {
    layer->OnDetach();
    delete layer;
  }
}

void LayerStack::PushLayer(Layer* layer) {
  MI_CORE_ASSERT(layer, "PushLayer called with null pointer!");
  m_Layers.emplace(m_Layers.begin() + _layerInsertIndex, layer);
  _layerInsertIndex++;
}

void LayerStack::PushOverlay(Layer* overlay) {
  MI_CORE_ASSERT(overlay, "PushOverlay called with null pointer!");
  m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer) {
  auto it =
      std::find(m_Layers.begin(), m_Layers.begin() + _layerInsertIndex, layer);
  if (it != m_Layers.begin() + _layerInsertIndex) {
    layer->OnDetach();
    m_Layers.erase(it);
    _layerInsertIndex--;
    delete layer;
  }
}

void LayerStack::PopOverlay(Layer* overlay) {
  auto it =
      std::find(m_Layers.begin() + _layerInsertIndex, m_Layers.end(), overlay);
  if (it != m_Layers.end()) {
    overlay->OnDetach();
    m_Layers.erase(it);
    delete overlay;
  }
}
}  // namespace Mochii
