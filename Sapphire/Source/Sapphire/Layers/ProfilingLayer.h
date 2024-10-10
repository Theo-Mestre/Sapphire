#ifndef SPH_PROFILING_LAYER_H
#define SPH_PROFILING_LAYER_H

#include "Layer.h"
#include "Sapphire/Profiling/FrameTimeData.h"

namespace sph
{
	class ProfilingLayer
		: public Layer
	{
	public:
		ProfilingLayer();
		virtual ~ProfilingLayer() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime _dt) override;
		virtual void OnEvent(Event& event) override {}
		virtual void OnImGuiRender() override;

	private:
		Scope<Profiling::FrameTimeData> m_frameTimeData;
	};
}
#endif