#include "sphpch.h"
#include "ProfilingLayer.h"
#include "Sapphire/Renderer/Renderer2D.h"

namespace sph
{
	ProfilingLayer::ProfilingLayer()
		: Layer("ProfilingLayer")
	{
		SPH_PROFILE_FUNCTION();
	}

	ProfilingLayer::~ProfilingLayer()
	{
		SPH_PROFILE_FUNCTION();
	}

	void ProfilingLayer::OnAttach()
	{
		SPH_PROFILE_FUNCTION();

		m_frameTimeData = CreateScope<Profiling::FrameTimeData>();
	}

	void ProfilingLayer::OnDetach()
	{
		SPH_PROFILE_FUNCTION();

		m_frameTimeData.reset();
	}

	void ProfilingLayer::OnUpdate(DeltaTime _dt)
	{
		SPH_PROFILE_FUNCTION();

		m_frameTimeData->Update(Time::DeltaTime, Time::GameTime);
	}

	void ProfilingLayer::OnImGuiRender()
	{
		SPH_PROFILE_FUNCTION();

		ImGui::Begin("Metrics");
		ImGui::Text(" Frame Time: %f", Time::DeltaTime);
		ImGui::Text(" FPS: %f", 1.f / Time::DeltaTime);
		ImGui::Text(" Quad number : %d", Renderer2D::Stats::QuadCount);
		ImGui::Text(" Draw call : %d", Renderer2D::Stats:: DrawCalls);
		ImGui::SetWindowSize(ImVec2(430, 290));
		if (ImPlot::BeginPlot("Frame Time"))
		{
			ImPlot::GetStyle().PlotBorderSize = 0;
			ImPlot::GetStyle().PlotDefaultSize = ImVec2(300, 200);

			ImPlot::SetupAxisLimits(ImAxis_X1, sph::Time::GameTime - 2, sph::Time::GameTime, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_X1, sph::Time::GameTime - 2, sph::Time::GameTime, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0.f, 0.125f);

			ImPlot::SetupAxes("Game time", "Frame time");
			ImPlot::PlotLine("Frame Time", m_frameTimeData->GetGameTimeData(), m_frameTimeData->GetFrameTimeData(), (uint32_t)m_frameTimeData->GetFrameTimeDataSize(), ImPlotLineFlags_::ImPlotLineFlags_Shaded);
			ImPlot::EndPlot();
		}
		ImGui::End();
	}
}