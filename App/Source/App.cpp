#include "Sapphire.h"
#include "Sapphire/Core/EntryPoint.h"

#include "Layers/TestRenderer.h"

class Sandbox2D
	: public sph::Application
{
public:
	Sandbox2D()
	{
		PushLayer(new TestRenderer());
	}
};

sph::Application* sph::CreateApplication()
{
	return new Sandbox2D();
}

/*//////////////// ImGui Code //////////////
//// Note: Switch this to true to enable dockspace

//static bool dockspaceOpen = true;
//bool opt_fullscreen = true;
//static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

//// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
//// because it would be confusing to have two docking targets within each others.
//ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
//if (opt_fullscreen)
//{
//	ImGuiViewport* viewport = ImGui::GetMainViewport();
//	ImGui::SetNextWindowPos(viewport->Pos);
//	ImGui::SetNextWindowSize(viewport->Size);
//	ImGui::SetNextWindowViewport(viewport->ID);
//	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
//	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
//	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
//	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
//}

//// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
//if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
//	window_flags |= ImGuiWindowFlags_NoBackground;

//// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
//// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
//// all active windows docked into it will lose their parent and become undocked.
//// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
//// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
//ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
//ImGui::PopStyleVar();

//if (opt_fullscreen) ImGui::PopStyleVar(2);

////////////////  DockSpace //////////////
//ImGuiIO& io = ImGui::GetIO();
//if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
//{
//	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
//	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
//}

//if (ImGui::BeginMenuBar())
//{
//	if (ImGui::BeginMenu("File"))
//	{
//		if (ImGui::MenuItem("Exit")) // Add exit function here
//			ImGui::EndMenu();
//	}

//	ImGui::EndMenuBar();
//}

//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
//ImGui::Begin("Viewport");
//ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

//uint32_t textureID = m_framebuffer->GetColorAttachmentRendererID();
//ImVec2 windowSize = { (float)m_framebuffer->GetSpecification().Width, (float)m_framebuffer->GetSpecification().Height };
//ImGui::Image((void*)textureID, windowSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
//ImGui::End();
//ImGui::PopStyleVar();

//ImGui::End();*/