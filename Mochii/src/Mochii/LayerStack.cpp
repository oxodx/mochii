#include "mzpch.h"
#include "LayerStack.h"

namespace Mochii {
	LayerStack::LayerStack() {
	
	}

	LayerStack::~LayerStack() {
		for (Layer* layer : _Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer) {
		_Layers.emplace(_Layers.begin() + _LayerInsertIndex, layer);
		_LayerInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(_Layers.begin(), _Layers.begin() + _LayerInsertIndex, layer);
		if (it != _Layers.begin() + _LayerInsertIndex) {
			layer->OnDetach();
			_Layers.erase(it);
			_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(_Layers.begin() + _LayerInsertIndex, _Layers.end(), overlay);
		if (it != _Layers.end()) {
			overlay->OnDetach();
			_Layers.erase(it);
		}
	}
}
