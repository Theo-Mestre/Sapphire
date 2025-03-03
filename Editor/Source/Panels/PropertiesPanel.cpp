#include "sphpch.h"

#include "Sapphire/Scene/Entity.h"
#include "Sapphire/Scene/Components.h"

#include "PropertiesPanel.h"
#include "Panels/SceneHierarchyPanel.h"

namespace sph
{
	PropertiesPanel::PropertiesPanel(const Ref<SceneHierarchyPanel>& _context)
		: m_context(nullptr)
	{
		SetContext(_context);
	}

	PropertiesPanel::~PropertiesPanel()
	{
	}

	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::Begin("Properties");

		if (m_context == nullptr)
		{
			ImGui::Text("No context selected");
			ImGui::End();
			return;
		}

		if (!m_context->m_selectedEntity.IsValid())
		{
			ImGui::Text("No entity selected");
			ImGui::End();
			return;
		}

		DrawComponents(m_context->m_selectedEntity);

		ImGui::End();
	}

	void PropertiesPanel::SetContext(const Ref<SceneHierarchyPanel>& _context)
	{
		m_context = _context;
	}

	void PropertiesPanel::DrawComponents(Entity _entity)
	{
		if (_entity.HasComponent<TagComponent>())
		{
			auto& tag = _entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::Separator();

		if (_entity.HasComponent<TransformComponent>())
		{
			auto& transform = _entity.GetComponent<TransformComponent>().Transform;
			ImGui::DragFloat3("Position", &transform[3].x, 0.1f);
		}

		ImGui::Separator();

		if (_entity.HasComponent<CameraComponent>())
		{
			auto& cameraComponent = _entity.GetComponent<CameraComponent>();

			ImGui::Checkbox("Primary", &cameraComponent.IsPrimary);

			ImGui::Separator();

			auto& camera = cameraComponent.Camera;

			ImGui::Text("Camera Properties");

			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Orthographic Size", &orthoSize))
			{
				camera.SetOrthographicSize(orthoSize);
			}

			float aspectRatio = camera.GetAspectRatio();

			if (ImGui::DragFloat("Aspect Ratio", &aspectRatio))
			{
				camera.SetAspectRatio(aspectRatio);
			}

			ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.IsFixedAspectRatio);

			ImGui::Separator();

			if (ImGui::Button("Reset Camera"))
			{
				camera.SetOrthographicSize(10.0f);
				camera.SetAspectRatio(16.0f / 9.0f);
			}
		}

		if (_entity.HasComponent<SpriteRendererComponent>())
		{
			auto& sprite = _entity.GetComponent<SpriteRendererComponent>();
			ImGui::ColorEdit4("Color", glm::value_ptr(sprite.Color));
		}
	}
}