# Sapphire

Sapphire is a game engine developed in C++ using OpenGL. It started as a personal project to explore game engine architecture and learn how to manage a multi-system application. Inspired by The [Cherno's game engine series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT), this project has been a hands-on journey into graphics programming, rendering techniques, and engine development.

You can check out the presentation I made for this project [here](https://docs.google.com/presentation/d/1hBBXi7OgnB5CnnUq6ogenfiARTrwbtATjnQTT7INczE/edit?usp=sharing). 

## Why I Built This
I wanted to understand how a game engine works under the hood while also tackling the challenges of structuring a multi-system application. Rather than focusing purely on the complexity of code, my goal was to design an engine that effectively integrates different components, such as rendering, scene management, and EditorUI.

## Development Journey
1. **Core Engine:** I began by implementing the core application that I could rely on to build the redenring features.
2. **Graphics Features:** I progressively added: 
   - Quad rendering
   - Texturing & blending
   - Batch rendering
   - Frame buffers
3. **Lighting System:** I experimented with a simple shader-based lighting system to push my understanding of real-time rendering.
4. **Scene Management & Editor:** Currently, I am developing a scene management system and an editor to make the engine more practical for game development.

<img width="2559" height="1326" alt="Untitled" src="https://github.com/user-attachments/assets/0e2ddb14-8b28-4534-ab1b-cabf391fa49d" />

## Installation
To get started, clone the repository:
```sh
git clone https://github.com/Theo-Mestre/sapphire.git
```
Then, build the project using the `SetupWindows.bat` script located inside the `Scripts` folder.

## Tech Stack
- **Language:** C++20
- **Rendering:** OpenGL 4.5

## Dependencies
- [GLFW](https://github.com/glfw/glfw) (Windowing and Input)
- [Glad](https://github.com/Dav1dde/glad) (OpenGL Loader)
- [Spdlog](https://github.com/gabime/spdlog) (Logging)
- [ImGui](https://github.com/ocornut/imgui) & [ImPlot ](https://github.com/epezent/implot) (UI Framework)
- [GLM](https://github.com/g-truc/glm) (Mathematics Library)
- [EnTT](https://github.com/skypjack/entt) (Entity Component System)
- [yaml-cpp]([https://github.com/skypjack/entt](https://github.com/jbeder/yaml-cpp)) (parser and emitter)

## License
This project is licensed under the MIT License - see the LICENSE file for details.


