#include "sphpch.h"
#include <entt/entt.hpp>
#include <imgui.h>
#include <imgui_internal.h>


#include "SceneHierarchyPanel.h"
#include "Sapphire/Scene/Scene.h"
#include "Sapphire/Scene/Components.h"

namespace sph
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& _context)
		: m_context(nullptr)
		, m_selectedEntity()
	{
		SetContext(_context);
	}

	SceneHierarchyPanel::~SceneHierarchyPanel()
	{
		m_context = nullptr;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (m_context == nullptr) return;

		ImGui::Begin("Scene Hierarchy");

		auto view = m_context->m_registry.view<TagComponent>();

		int i = 0;
		for (auto _entityID : view)
		{
			Entity entity = { _entityID, m_context.get() };

			ImGui::PushID(i++);
			DrawEntityNode(entity);
			ImGui::PopID();
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_selectedEntity = {};
		}

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_context->CreateEntity("Empty Entity");
			}
		
			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& _context)
	{
		m_context = _context;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity _entity)
	{
		auto& tag = _entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((_entity == m_selectedEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool isOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)_entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_selectedEntity = _entity;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				m_context->DestroyEntity(_entity);
				if (m_selectedEntity == _entity)
				{
					m_selectedEntity = {};
				}
			}
			ImGui::EndPopup();
		}

		if (!isOpened) return;

		flags = ImGuiTreeNodeFlags_OpenOnArrow;
		isOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)_entity, flags, tag.c_str());

		if (isOpened)
		{
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}