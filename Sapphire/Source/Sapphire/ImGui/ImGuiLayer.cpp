#include "sphpch.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "implot.h"

#include "ImGuiLayer.h"
#include "Sapphire/Window/Window.h"

#include "Sapphire/Events/ApplicationEvent.h"
#include "Sapphire/Events/KeyEvent.h"
#include "Sapphire/Events/MouseEvent.h"
#include "Sapphire/Core/Application.h"
#include "Sapphire/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace sph
{
	ImGuiLayer::ImGuiLayer(Window& _window)
		: Layer("ImGuiLayer")
		, m_window(_window)
	{
		SPH_PROFILE_FUNCTION();

	}

	ImGuiLayer::~ImGuiLayer()
	{
		SPH_PROFILE_FUNCTION();

	}

	void ImGuiLayer::OnAttach()
	{
		SPH_PROFILE_FUNCTION();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		(void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();


		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			//style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(m_window.GetNativeWindow()), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		SPH_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate(DeltaTime _dt)
	{
		SPH_PROFILE_FUNCTION();

	}

	void ImGuiLayer::OnEvent(Event& _event)
	{
		SPH_PROFILE_FUNCTION();

		if (!m_isViewportFocused || !m_isViewportHovered)
		{
			auto io = ImGui::GetIO();
			_event.Handled |= _event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			_event.Handled |= _event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		SPH_PROFILE_FUNCTION();

		ImGui::Begin("Settings");

		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", Renderer::Stats::DrawCalls);
		ImGui::Text("Quads: %d", Renderer::Stats::QuadCount);

		ImGui::Text("Press U to switch from docking to fullscreen");
		ImGui::End();
	}

	void ImGuiLayer::Begin()
	{
		SPH_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		SPH_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)m_window.GetWidth(), (float)m_window.GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			glfwMakeContextCurrent(backup_current_context);
		}
	}
}