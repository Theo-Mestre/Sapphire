#ifndef SPH_ENGINE_INCLUDES_H
#define SPH_ENGINE_INCLUDES_H

#include "Sapphire/Core/Core.h"
#include "Sapphire/Core/Application.h"
#include "Sapphire/Core/Time.h"

#pragma warning(push, 0)
#include "Sapphire/Core/Log.h"
#pragma warning(pop)

#include "Sapphire/Events/ApplicationEvent.h"
#include "Sapphire/Events/KeyEvent.h"
#include "Sapphire/Events/MouseEvent.h"
#include "Sapphire/Core/Input.h"
#include "Sapphire/Core/KeyCode.h"
#include "Sapphire/Core/MouseButton.h"

#include "Sapphire/Layers/LayerStack.h"
#include "Sapphire/Window/Window.h"	

#include "Sapphire/Renderer/RenderCommand.h"
#include "Sapphire/Renderer/Renderering2D.h"
#include "Sapphire/Renderer/VertexArray.h"
#include "Sapphire/Renderer/Shader.h"
#include "Sapphire/Renderer/Material.h"
#include "Sapphire/Renderer/Buffer.h"
#include "Sapphire/Renderer/UniformBuffer.h"
#include "Sapphire/Renderer/Framebuffer.h"
#include "Sapphire/Renderer/Texture.h"
#include "Sapphire/Renderer/SubTexture2D.h"
#include "Sapphire/Renderer/Sprite.h"
#include "Sapphire/Renderer/TileMap.h"

#include "Sapphire/Scene/Scene.h"
#include "Sapphire/Scene/Entity.h"
#include "Sapphire/Scene/Components.h"

#include "Sapphire/Renderer/Camera.h"

#include "Sapphire/Profiling/Profiler.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <implot.h>
#endif