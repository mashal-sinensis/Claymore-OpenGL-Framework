## Developer Installation

# You may follow the OpenGL Setup steps from: 
https://www.youtube.com/watch?v=XpBGwZNyUh0&ab_channel=VictorGordan

# **OR**

# You can follow these project-specific written steps (based on the tutorial above):

1. (Optional) Update Graphics Card Drivers
2. Download Visual Studio Community IDE
3. Download Windows x64 Installer for cmake (cmake.org/download)
4. Download the Source Package for GLFW (glfw.org/download.html)
5. Go to glad.dav1d.de:
    a. Select gl Version 3.3
    b. Ensure that Language is set to C/C++
    c. Specification is set to OpenGL
    d. Profile is set to Core
    e. Ignore everything else and press "Generate"
    f. Click glad.zip to download it
6. Open Visual Studio and Select Empty Project for C++ and select "Place solution and project in the same directory"
7. Create folders in the project solution directory to make your project directory look like this:
<br/>
.<br/>
└── Project Solution Directory/<br/>
&nbsp&nbsp&nbsp&nbsp├── [ProjectName].sln<br/>
&nbsp&nbsp&nbsp&nbsp├── [ProjectName].vcxproj<br/>
&nbsp&nbsp&nbsp&nbsp├── [ProjectName].vcxproj.filters<br/>
&nbsp&nbsp&nbsp&nbsp├── [ProjectName].vcxproj.user<br/>
&nbsp&nbsp&nbsp&nbsp└── Libraries/<br/>
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp├── lib<br/>
&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp└── include<br/>
<br/>
9. Extract glfw-3.3.2 from the glfw-3.3.2.zip
10. Open cmake and set the build directory to glfw-3.3.2/build (create the folder) and the source directory to glfw-3.3.2
11. Click on Configure (ensure that the correct Visual Studio generator is selected, use default native compilers) and click Finish
12. Some checkboxes appear, ensure that the following checkboxes are set to true: GLFW_BUILD_DOCS, GLFW_BUILD_EXAMPLES, GLFW_BUILD_TESTS, GLFW_INSTALL, USE_MSVC_RUNTIME_LIBRARY_DLL (and the rest are set to false)
13. Click Generate and exit Cmake once its done.
14. Open the generated build folder (glfw-3.3.2/build), open GLFW.sln in VS, then build the solution. Exit VS when it's done.
15. Open the debug folder (glfw-3.3.2/build/src/Debug) and copy the glfw3.lib file into the project lib folder (ProjectSolutionDirectory/Libraries/lib)
16. Open the include folder (glfw-3.3.2/include) and copy the GLFW folder into the project include folder (ProjectSolutionDirectory/Libraries/include)
17. Open glad.zip, open glad.zip/include and extract glad and KHR into the project include folder (ProjectSolutionDirectory/Libraries/include)
18. Open glad.zip/src and extract the glad.c file into your project directory (ProjectSolutionDirectory/)
19. Now, you can open the project solution ([ProjectName].sln) to configure it:
    a. Ensure that you have x64 selected, click the Project button at the very top (next to Git and Build), select All Platforms
    b. Go to VC++ Directories, open Include Directories and add the project include folder (ProjectSolutionDirectory/Libraries/include)
    c. Do the same for the Library Directories, add the project lib folder (ProjectSolutionDirectory/Libraries/lib)
    d. Open Linker/Input and open Additional Dependencies, write "glfw3.lib" and "opengl32.lib" in separate lines.
20. Next, drag the glad.c file from your project directory into the solution source files (so VC can detect it)
21. For textures to work, you need to download stb_image.h from github.com/nothings/stb:
    a. Create a folder in the project solution directory (ProjectSolutionDirectory/Libraries/include/stb)
    b. Download stb_image.h into the folder (github.com/nothings/stb)
    c. In the next step, once you have cloned the github project files, right click the stb.cpp file and click "Compile", and you're done.
23. Lastly, you can clone the github project files into the project directory and similarly drag the .cpp and .h files into the VS project
24. Now, you should be able to run and build the project
