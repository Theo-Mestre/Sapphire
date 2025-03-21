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

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		float fontSize = 18.0f;
		io.Fonts->AddFontFromFileTTF("Fonts/OpenSans-Bold.ttf", fontSize);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Fonts/OpenSans-Regular.ttf", fontSize);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		SetThemeColors();
		SetStyle();

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
	}

	void ImGuiLayer::Begin()
	{
		SPH_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
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

	void ImGuiLayer::SetThemeColors(const ImGuiThemeColor& _themeColor)
	{
		auto& colors = ImGui::GetStyle().Colors;

		for (auto& [key, value] : _themeColor.Colors)
		{
			if (key < 0 || key >= ImGuiCol_COUNT) continue;

			colors[key] = value;
		}
	}

	void ImGuiLayer::SetStyle()
	{
		auto& style = ImGui::GetStyle();

		style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.WindowPadding = ImVec2(5.0f, 5.0f);
		style.WindowRounding = 5.0f;
		style.WindowBorderSize = 0.0f;

		style.TabRounding = 4.0f;
		style.GrabRounding = 4.0f;

		style.ScrollbarSize = 10.0f;
		style.ScrollbarRounding = 10.0f;

		style.IndentSpacing = 0.0f;
		style.ItemSpacing = ImVec2(5.0f, 10.0f);
		style.FramePadding = ImVec2(5.0f, 2.0f);
		style.FrameRounding = 5.0f;

		style.PopupBorderSize = 0.0f;
		style.PopupRounding = 5.0f;

		style.DockingSeparatorSize = 1.0f;
	}

	ImGuiThemeColor ImGuiThemeColor::Default()
	{
		ImGuiThemeColor theme;

		theme.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);

		// Headers
		theme.Colors[ImGuiCol_Header] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
		theme.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
		theme.Colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);

		// Buttons
		theme.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
		theme.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
		theme.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);

		// Frame BG
		theme.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
		theme.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
		theme.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);

		// Tabs
		theme.Colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
		theme.Colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.3805f, 0.381f, 1.0f);
		theme.Colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);
		theme.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
		theme.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);

		// Title
		theme.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
		theme.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
		theme.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);

		// Popups
		theme.Colors[ImGuiCol_PopupBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.0f);
		theme.Colors[ImGuiCol_Border] = ImVec4(0.22f, 0.22f, 0.22f, 1.0f);

		theme.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.43f, 0.69f, 1.0f, 1.0f);
		theme.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

		theme.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);

		return theme;
	}
}