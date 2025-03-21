#include "sphpch.h"
#include <imgui_internal.h>

#include "Sapphire/Scene/Entity.h"
#include "Sapphire/Scene/Components.h"
#include "Sapphire/Renderer/Texture.h"

#include "PropertiesPanel.h"
#include "Panels/SceneHierarchyPanel.h"

namespace sph
{
	extern const std::filesystem::path g_AssetPath;

	struct Vec3Button
	{
		ImVec4 ButtonColor;
		ImVec4 HoveredButtonColor;
		ImVec4 ActiveButtonColor;
	};

	constexpr static Vec3Button XSelector = { ImVec4(0.8f, 0.1f, 0.15f, 1.0f), ImVec4(0.9f, 0.2f, 0.2f, 1.0f), ImVec4(0.8f, 0.1f, 0.15f, 1.0f) };
	constexpr static Vec3Button YSelector = { ImVec4(0.2f, 0.7f, 0.2f, 1.0f), ImVec4(0.3f, 0.8f, 0.3f, 1.0f), ImVec4(0.2f, 0.7f, 0.2f, 1.0f) };
	constexpr static Vec3Button ZSelector = { ImVec4(0.1f, 0.25f, 0.8f, 1.0f), ImVec4(0.2f, 0.35f, 0.9f, 1.0f), ImVec4(0.1f, 0.25f, 0.8f, 1.0f) };

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

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& _name, Entity _entity, UIFunction _uiFunction)
	{
		bool removeComponent = false;

		if (_entity.HasComponent<T>() == false) return;

		const ImGuiTreeNodeFlags treeNodeFlags =
			ImGuiTreeNodeFlags_DefaultOpen |
			ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_AllowItemOverlap |
			ImGuiTreeNodeFlags_FramePadding;

		auto& component = _entity.GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, _name.c_str());
		ImGui::PopStyleVar();

		if (!(typeid(T) == typeid(TransformComponent)))
		{
			ImGui::SameLine(contentRegionAvailable.x - lineHeight);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("Component Settings");
			}

			if (ImGui::BeginPopup("Component Settings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					_entity.RemoveComponent<T>();
					ImGui::EndPopup();
					if (open)
					{
						ImGui::TreePop();
					}
					return;
				}

				ImGui::EndPopup();
			}
		}

		if (open)
		{
			_uiFunction(component);
			ImGui::TreePop();
		}
	}

	static void DrawVec3Control(const std::string& _label, glm::vec3& _values, float _resetValue = 0.0f, float _columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(_label.c_str());

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, _columnWidth);
		ImGui::Text(_label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 4.0f, lineHeight };

		ImGui::PushTabStop(false);
		{
			ImGui::PushFont(boldFont);
			ImGui::PushStyleColor(ImGuiCol_Button, XSelector.ButtonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, XSelector.HoveredButtonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, XSelector.ActiveButtonColor);

			if (ImGui::Button("X", buttonSize)) _values.x = _resetValue;

			ImGui::PopStyleColor(3);
			ImGui::PopFont();
		}
		ImGui::PopTabStop();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &_values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushTabStop(false);
		{
			ImGui::PushFont(boldFont);
			ImGui::PushStyleColor(ImGuiCol_Button, YSelector.ButtonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, YSelector.HoveredButtonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, YSelector.ActiveButtonColor);

			if (ImGui::Button("Y", buttonSize)) _values.y = _resetValue;

			ImGui::PopStyleColor(3);
			ImGui::PopFont();
		}
		ImGui::PopTabStop();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &_values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushTabStop(false);
		{
			ImGui::PushFont(boldFont);
			ImGui::PushStyleColor(ImGuiCol_Button, ZSelector.ButtonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ZSelector.HoveredButtonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ZSelector.ActiveButtonColor);

			if (ImGui::Button("Z", buttonSize)) _values.z = _resetValue;

			ImGui::PopStyleColor(3);
			ImGui::PopFont();
		}
		ImGui::PopTabStop();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &_values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::Columns(1);
		ImGui::PopStyleVar(2);

		ImGui::PopID();
	}

	void PropertiesPanel::DrawComponents(Entity _entity)
	{
		if (_entity.HasComponent<TagComponent>())
		{
			auto& tag = _entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			ImGui::PushItemWidth(ImGui::GetWindowContentRegionMax().x - 125.0f);
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
			ImGui::PopItemWidth();
		}

		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 115.0f);
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component", ImVec2(115.0f, 0.0f)))
		{
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_context->m_selectedEntity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_context->m_selectedEntity.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Animator"))
			{
				m_context->m_selectedEntity.AddComponent<SpriteAnimatorComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Parallax Component"))
			{
				m_context->m_selectedEntity.AddComponent<ParallaxComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Player Controller"))
			{
				m_context->m_selectedEntity.AddComponent<PlayerController>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", _entity, [](auto& _component)
			{
				DrawVec3Control("Translation", _component.Translation);
				glm::vec3 rotation = glm::degrees(_component.Rotation);
				DrawVec3Control("Rotation", rotation);
				_component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", _component.Scale, 1.0f);
				ImGui::Spacing();
			});

		DrawComponent<CameraComponent>("Camera", _entity, [](auto& _component)
			{
				SceneCamera& camera = _component.Camera;

				ImGui::Checkbox("Primary", &_component.IsPrimary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov)) camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear)) camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar)) camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize)) camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNear();
					if (ImGui::DragFloat("Near", &orthoNear)) camera.SetOrthographicNear(orthoNear);

					float orthoFar = camera.GetOrthographicFar();
					if (ImGui::DragFloat("Far", &orthoFar)) camera.SetOrthographicFar(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &_component.IsFixedAspectRatio);
				}
			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", _entity, [](auto& _component)
			{
				ImGui::Text("Texture");

				if (_component.Texture == nullptr)
				{
					ImGui::Button("No texture", ImVec2(100.0f, 0.0f));
				}
				else
				{
					ImGui::ImageButton((ImTextureID)(int64_t)_component.Texture->GetRendererID(), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_DRAWER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
						_component.Texture = Texture2D::Create(texturePath.string());
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::Text("Color");
				ImGui::ColorEdit4("Color", glm::value_ptr(_component.Color));

				ImGui::DragFloat("Tiling Factor", &_component.TilingFactor, 0.1f, 0.0f, 100.0f);
			});

		DrawComponent<SpriteAnimatorComponent>("Sprite Animator", _entity, [](auto& _component)
			{
				ImGui::DragInt2("Frame Count", glm::value_ptr(_component.FrameCount));
				ImGui::DragFloat("Frame Time", &_component.FrameTime, 0.01f, 0.0f, 10.0f);
				ImGui::Checkbox("Looping", &_component.IsLooping);

				ImGui::Spacing();
			});
		
		DrawComponent<ParallaxComponent>("Parallax Component", _entity, [](ParallaxComponent& _component)
			{
				ImGui::DragFloat2("Initial Position", glm::value_ptr(_component.InitialPosition), 0.01f);
				ImGui::DragFloat2("Speed", glm::value_ptr(_component.Speed), 0.01f);
				ImGui::DragFloat2("Threshold", glm::value_ptr(_component.Threshold), 0.01f);

				ImGui::Spacing();
			});

		DrawComponent<PlayerController>("Player Controller", _entity, [](PlayerController& _component)
			{
				ImGui::DragFloat2("Camera Limits", glm::value_ptr(_component.m_cameraLimits), 0.01f);
				ImGui::DragFloat2("Player Limits", glm::value_ptr(_component.m_playerLimits), 0.01f);
				ImGui::DragFloat("Speed", &_component.m_speed, 0.01f);
				ImGui::Spacing();
			});
	}
}