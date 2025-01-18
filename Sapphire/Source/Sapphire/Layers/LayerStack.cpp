#include "sphpch.h"
#include "LayerStack.h"

namespace sph
{
	LayerStack::LayerStack()
	{
		SPH_PROFILE_FUNCTION();
	}

	LayerStack::~LayerStack()
	{
		SPH_PROFILE_FUNCTION();

		for (Layer* layer : m_layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* _layer)
	{
		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, _layer);
		_layer->OnAttach();
		m_layerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* _overlay)
	{
		SPH_PROFILE_FUNCTION();

		m_layers.emplace_back(_overlay);
		_overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* _layer)
	{
		SPH_PROFILE_FUNCTION();

		auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, _layer);
		if (it != m_layers.begin() + m_layerInsertIndex)
		{
			_layer->OnDetach();
			m_layers.erase(it);
			m_layerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* _overlay)
	{
		SPH_PROFILE_FUNCTION();

		auto it = std::find(m_layers.begin() + m_layerInsertIndex, m_layers.end(), _overlay);
		if (it != m_layers.end())
		{
			_overlay->OnDetach();
			m_layers.erase(it);
		}
	}

	void LayerStack::Clear()
	{
		SPH_PROFILE_FUNCTION();

		for (Layer* layer : m_layers)
		{
			layer->OnDetach();
			delete layer;
		}
		m_layers.clear();
		m_layerInsertIndex = 0;
	}
}
