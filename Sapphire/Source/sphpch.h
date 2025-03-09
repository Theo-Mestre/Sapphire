#ifndef SPH_PCH_H
#define SPH_PCH_H

#include <iostream>
#include <memory>
#include <functional>
#include <algorithm>

#include <string>
#include <array>
#include <vector>
#include <cstring>
#include <cstdint>

#include <fstream>

#include <chrono>


#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <entt/entt.hpp>

#include <imgui.h>
#include <implot.h>

#include <yaml-cpp/yaml.h>
#pragma warning(pop)

#include "Sapphire/Profiling/Profiler.h"
#endif