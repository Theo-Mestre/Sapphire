# Sapphire

![Language](https://img.shields.io/badge/C%2B%2B-20-blue) ![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey) ![License](https://img.shields.io/github/license/Theo-Mestre/Sapphire)

Sapphire is a 2D game engine written in C++20 on top of OpenGL 4.5. It started as a way to understand what actually happens under the hood of engines like Unity, and grew into a reasonably complete system covering rendering, scene management, an ECS-driven entity model, and a full editor built with ImGui.

The architecture is layered and modular. The core engine handles windowing, input, events and the application loop. On top of that sits a renderer that supports both a simple single-quad path and a full batch renderer. Scenes are composed of entities with components managed through EnTT, serialized to YAML, and editable live in the editor. The editor itself is a proper ImGui application with docking, a viewport, a scene hierarchy, an entity inspector, a content browser, and gizmo-based transform manipulation.

You can check out the original presentation [here](https://docs.google.com/presentation/d/1hBBXi7OgnB5CnnUq6ogenfiARTrwbtATjnQTT7INczE/edit?usp=sharing).

<img width="2559" height="1326" alt="Untitled" src="https://github.com/user-attachments/assets/0e2ddb14-8b28-4534-ab1b-cabf391fa49d" />

---

## Features

The renderer supports batch rendering up to 10,000 quads per draw call with 32 texture slots, subtexture support for spritesheets, and a tilemap renderer built on top of the same batch pipeline. There is also a profiling layer that plots live frame time data using ImPlot and writes instrumentation traces compatible with Chrome's tracing viewer.

The editor supports play/edit mode switching, mouse picking via a red-integer framebuffer attachment, drag-and-drop asset loading, and keyboard shortcuts for scene IO and gizmo switching.

---

## Getting Started

Clone the repository:

```sh
git clone https://github.com/Theo-Mestre/sapphire.git
```

Run the setup script from the `Scripts` folder to generate the Visual Studio 2022 solution:

```sh
Scripts/SetupWindows.bat
```

Open the generated `.sln` file, set `Editor` as the startup project, and build. The working directory for the debugger is set to `../Assets` by the build scripts, so assets should be placed there.

To clean the solution and regenerate it from scratch, run `Scripts/CleanSolution.bat` first.

> Sapphire currently only supports Windows x64 and requires OpenGL 4.5.

---

## Tech Stack

- [GLFW](https://github.com/glfw/glfw) (Windowing and Input)
- [Glad](https://github.com/Dav1dde/glad) (OpenGL Loader)
- [Spdlog](https://github.com/gabime/spdlog) (Logging)
- [ImGui](https://github.com/ocornut/imgui) & [ImPlot ](https://github.com/epezent/implot) (UI Framework)
- [GLM](https://github.com/g-truc/glm) (Mathematics Library)
- [EnTT](https://github.com/skypjack/entt) (Entity Component System)
- [yaml-cpp]([https://github.com/skypjack/entt](https://github.com/jbeder/yaml-cpp)) (parser and emitter)
- [Premake](https://premake.github.io/) (build configuration)

---

## Architecture Overview

A full breakdown of the main systems is available in [DOCUMENTATION.md](DOCUMENTATION.md).

---

## License

MIT, see [LICENSE](LICENSE) for details.
