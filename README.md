## How to run and overview
Rendering template aka graphics engine with phong shading model, texture support, scene graph, camera, geometric primitives and .obj loader implemented using GLFW core (cross-platform OpenGL API library, which simplifies the process of window creation, dealing with input devices etc., sort of a better GLUT version), GLAD- seeks for OpenGL function pointers in system libraries, and GLM- for matrix algebra.   

My implementations are located in /App directory, and the dependencies are in /core.
Implemented in Visual Studio, so I am supplementing it with .vcxproj and .sln. .vcxproj is the most important file as it contains some global preprocessor definitions. To compile, simply open the .sln in Visual Studio and build. GLFW is a part of the solution. I have removed some headers and sources from GLFW which are relevant to MacOS, Unix, etc., so even if you wish to make your own CMake files and try to compile it on some Linux machine, it won’t work. You need to get the source of GLFW and throw the missing stuff back.

For image loading, I use the popular single-header library stb image.h, which simplifies decoding and works with almost all common formats. 

There are two scenes:
1. "Solar system" 
2. Dummy hierarchical scene with pyramids and sphere and cubes.  

Camera implements a full stack of movements: forward, backward, right, left, up, down, along with freelook using Eulerian geometry.

The scenes, including the Solar System scene, are implemented using a scene graph. In the Solar System, the Sun is the root node, the Earth is a child of the Sun, and the Moon is a child of the Earth.
For the Solar System scene, each node's world matrix is computed as  $W = W_{parent} \times B$.  
For the dummy scene, the transformation is slightly different: $W = W_{parent} \times P \times B$, and the final model matrix used for drawing is (same in both scenes) $M = W \times L$.

The dummy scene contains three stacked, scaled pyramids with the Earth placed on top. Each object is translated relative to its parent node, parent to children hierarchy is from bottom to top. The pyramids sway and rotate, and these transformations are shared from the bottom. The sway is amplified because the post matrix is applied at each node in the hierarchy. The Earth also performs its own self-rotation. The root of the whole scene, is a "dummy root", and scene also contains three cubes. One cube rotates around a dummy root, while the other two rotate around the first cube in diagonal orbits. If the dummy root is moved, the scene, including pyramids, moves together with it. The cubes also perform self-scaling using a sine function.

During rendering, the graph is traversed from top to bottom while transformations are accumulated. In total, I use four matrices per object:
1. Base matrix: Contains the coordinate-system transformations. Base matrices describe the structural relationship between parent and child nodes and are accumulated through the scene graph.
2. Local matrix: Unique to each object and not shared. It is used for object-specific transformations such as self-rotation, non-relative scaling, or non-relative translation. The local matrix is applied when computing the final model matrix for drawing.
3. World matrix: Stores the accumulated transformations of base matrices + post matrices from the root of the scene graph to the current node.
4. Post matrix: Used for cumulative animations (only in my personal scene), such as the sway effect. I can keep the base matrix unchanged by having a post matrix. For the Solar System scene, the base matrix + local was more than enough.

You can switch the scenes by pressing TAB
