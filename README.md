# Project Setup Guide

## Developer Installation

You can follow either of the two options below to set up your OpenGL development environment:

### Option 1: Follow the Video Tutorial
- [Victor Gordan's OpenGL Setup Tutorial](https://www.youtube.com/watch?v=XpBGwZNyUh0&ab_channel=VictorGordan)

### Option 2: Follow the Written Steps Below (Based on the Tutorial)

### Prerequisites
1. **(Optional)** Update your graphics card drivers.
2. Download and install **[Visual Studio Community](https://visualstudio.microsoft.com/)**.
3. Download the **Windows x64 Installer** for CMake from [cmake.org](https://cmake.org/download/).
4. Download the **Source Package** for GLFW from [glfw.org](https://www.glfw.org/download.html).
5. Go to [glad.dav1d.de](https://glad.dav1d.de) and configure the following:
    - **API**: OpenGL
    - **Version**: 3.3
    - **Profile**: Core
    - **Language**: C/C++
    - Click **Generate** and download the `glad.zip` file.

### Project Directory Structure
Create the following folder structure within your project directory:

```
.
└── ProjectSolutionDirectory/
    ├── [ProjectName].sln
    ├── [ProjectName].vcxproj
    ├── [ProjectName].vcxproj.filters
    ├── [ProjectName].vcxproj.user
    └── Libraries/
        ├── lib/
        └── include/
```

### Installation Steps

9. Extract `glfw-3.3.2` from the downloaded ZIP.
10. Open CMake and set:
    - **Source Directory**: `glfw-3.3.2`
    - **Build Directory**: `glfw-3.3.2/build` (create this folder if it doesn't exist)
11. Click **Configure** (ensure the correct Visual Studio generator is selected) and then click **Finish**.
12. In the CMake options, enable the following checkboxes:
    - `GLFW_BUILD_DOCS`
    - `GLFW_BUILD_EXAMPLES`
    - `GLFW_BUILD_TESTS`
    - `GLFW_INSTALL`
    - `USE_MSVC_RUNTIME_LIBRARY_DLL`
    - Ensure all other options are set to false.
13. Click **Generate** and exit CMake once the process is complete.
14. Open the generated `GLFW.sln` in Visual Studio, build the solution, and then close Visual Studio.
15. Copy the generated `glfw3.lib` file from `glfw-3.3.2/build/src/Debug` to `ProjectSolutionDirectory/Libraries/lib`.
16. Copy the `GLFW` folder from `glfw-3.3.2/include` to `ProjectSolutionDirectory/Libraries/include`.
17. Extract the `glad` and `KHR` folders from `glad.zip/include` into `ProjectSolutionDirectory/Libraries/include`.
18. Extract the `glad.c` file from `glad.zip/src` into `ProjectSolutionDirectory/`.

### Visual Studio Project Configuration

19. Open `[ProjectName].sln` and configure as follows:
    - **Platform**: Ensure `x64` is selected.
    - Go to **Project > Properties > VC++ Directories**:
        - **Include Directories**: Add `ProjectSolutionDirectory/Libraries/include`
        - **Library Directories**: Add `ProjectSolutionDirectory/Libraries/lib`
    - Go to **Linker > Input > Additional Dependencies** and add:
        - `glfw3.lib`
        - `opengl32.lib`
20. Drag `glad.c` into the **Source Files** filter in Visual Studio.

### Setting Up Textures with stb_image

21. Download `stb_image.h` from [github.com/nothings/stb](https://github.com/nothings/stb):
    - Create the folder `ProjectSolutionDirectory/Libraries/include/stb`
    - Place `stb_image.h` into this folder.
    - Upon cloning `stb.cpp` from the repo, right-click it in Visual Studio and select **Compile**.

### Finalizing the Project

22. Clone your project source files into the project directory and drag the `.cpp` and `.h` files into Visual Studio.
23. Build and run the project to ensure the setup is successful.

---

You're now ready to develop using OpenGL in Visual Studio! 🎉

