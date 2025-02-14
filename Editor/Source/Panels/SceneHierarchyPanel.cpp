#include "sphpch.h"
#include "SceneHierarchyPanel.h"

#include <entt/entt.hpp>

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
		for (auto _entityID : view)
		{
			Entity entity = { _entityID, m_context.get() };
			DrawEntityNode(entity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_selectedEntity = {};
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

		if (!isOpened) return;

		flags = ImGuiTreeNodeFlags_OpenOnArrow;
		isOpened = ImGui::TreeNodeEx((void*)1000, flags, tag.c_str());
		if (isOpened)
		{
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}