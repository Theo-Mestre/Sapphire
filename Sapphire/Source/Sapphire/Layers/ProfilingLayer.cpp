#include "sphpch.h"
#include "ProfilingLayer.h"

namespace sph
{
	ProfilingLayer::ProfilingLayer()
		: Layer("ProfilingLayer")
	{
	}

	ProfilingLayer::~ProfilingLayer()
	{
	}

	void ProfilingLayer::OnAttach()
	{
		m_frameTimeData = CreateScope<Profiling::FrameTimeData>();
	}

	void ProfilingLayer::OnDetach()
	{
		m_frameTimeData.reset();
	}

	void ProfilingLayer::OnUpdate(DeltaTime _dt)
	{
		m_frameTimeData->Update(Time::DeltaTime, Time::GameTime);
	}

	void ProfilingLayer::OnImGuiRender()
	{
		ImGui::Begin("Metrics");
		ImGui::Text(" Frame Time: %f", Time::DeltaTime);
		ImGui::Text(" FPS: %f", 1.f / Time::DeltaTime);
		ImGui::SetWindowSize(ImVec2(430, 255 ));
		if (ImPlot::BeginPlot("Frame Time"))
		{
			ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
			ImPlot::GetStyle().PlotBorderSize = 0;
			ImPlot::GetStyle().PlotDefaultSize = ImVec2(300, 200);

			ImPlot::SetupAxisLimits(ImAxis_X1, sph::Time::GameTime - 2, sph::Time::GameTime, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0.f, 0.125f);

			ImPlot::SetupAxes("Game time", "Frame time");
			ImPlot::PlotLine("Frame Time", m_frameTimeData->GetGameTimeData(), m_frameTimeData->GetFrameTimeData(), m_frameTimeData->GetFrameTimeDataSize(), ImPlotLineFlags_::ImPlotLineFlags_Shaded);
			ImPlot::EndPlot();
		}
		ImGui::End();
	}
}