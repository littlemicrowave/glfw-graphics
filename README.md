## How to run and overview
Rendering template aka graphics engine with phong shading model, texture support, scene graph, camera, geometric primitives and .obj loader implemented using GLFW core (cross-platform OpenGL API library, which simplifies the process of window creation, dealing with input devices etc., sort of a better GLUT version), GLAD- seeks for OpenGL function pointers in system libraries, and GLM- for matrix algebra.   

My implementations are located in /App directory, and the dependencies are in /core.
Implemented in Visual Studio, so I am supplementing it with .vcxproj and .sln. .vcxproj is the most important file as it contains some global preprocessor definitions. To compile, simply open the .sln in Visual Studio and build. GLFW is a part of the solution (no dynamic links). I have removed some headers and sources from GLFW which are relevant to MacOS, Unix, etc., so even if you wish to make your own CMake files and try to compile it on some Linux machine, it won’t work. You need to get the source of GLFW and throw the missing stuff back.

For image loading, I use the popular single-header library stb image.h, which simplifies decoding and works with almost all common formats. 

There are two scenes:
1. "Solar system" 
2. Dummy hierarchical scene with pyramids and sphere and cubes.  

You can switch the scenes by pressing TAB
