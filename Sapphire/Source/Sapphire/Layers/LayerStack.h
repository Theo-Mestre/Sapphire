#ifndef SAPP_LAYER_STACK_H
#define SAPP_LAYER_STACK_H

#include "Layer.h"

namespace sph
{
	/**
	 * @brief Stack of layers.
	 * The layers pushed will be managed by the LayerStack and deleted when the LayerStack is deleted.
	 * But if a layer is popped from the stack, its deletion is not managed by the LayerStack.
	 * So the deletion is the responsibility of the user.
	 * 
	 * Layer are pushed before overlays.
	 * The stack is updated from the bottom to the top so the overlays are updated first.
	 */
	class  LayerStack
	{
	public:
		using Iterator = std::vector<Layer*>::iterator;
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* _layer);
		void PushOverlay(Layer* _overlay);
		void PopLayer(Layer* _layer);
		void PopOverlay(Layer* _overlay);

		void Clear();

		Iterator begin() { return m_layers.begin(); }
		Iterator end() { return m_layers.end(); }
	private:
		std::vector<Layer*> m_layers;
		unsigned int m_layerInsertIndex = 0;
	};
}

#endif