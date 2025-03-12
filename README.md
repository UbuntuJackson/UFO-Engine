#UFO-Engine

Build instructions (Linux)

sudo apt install the following:
libopengl-dev
libglx-dev
libpng-dev
libx11-dev
libthread-pool-dev (Uncertain if necessary)
libopenal-dev (Uncertain if necessary)

-python 3.10+ (to generate the project)

Create a build folder.
Create a project.py file in the root directory

Use git submodule add https://github.com/UbuntuJackson/UFO-Engine.git

If on linux, use the following configurations:
```py
import importlib
tool = importlib.import_module("UFO-Engine.tools.UFOProjectManager")
project = tool.ProjectManager()
project.source_files.append("main.cpp")
project.generate_project()

#build takes arguments
#_cmake_args - Arguments for cmake, such as debug flags like -ggdb to set optimisation level
# for debugging with GDB
# #_generator_option - For example -G Ninja if Ninja should be used. Can be left empty if using make
#_make_or_ninja - ninja if using ninja. make if using make.
#_make_or_ninja_args - -j6 for using additional cores to compile. Works with both ninja and make.
result = project.build("-ggdb", "", "make" ,"-j6")

if not result: print("Build failed")
else: project.run()
```

Build instructions (Windows)

OpenGL 1.0 should already exist on Windows, no additional dependencies should be needed. Open an issue if this is discovered to be incorrect.

Create a build folder.
Create a project.py file in the root directory
If on windows, use the following configurations:
```py
import importlib
tool = importlib.import_module("UFO-Engine.tools.UFOProjectManager")
project = tool.ProjectManager()
project.source_files.append("main.cpp")
project.generate_project()
```
Then use the resulting CMakeLists.txt in Visual Studio to build a solution.

If you wish to use Ninja Build, then you can use the same project file (project.py) as for Linux except different arguments for project.build():
```py
import importlib
tool = importlib.import_module("UFO-Engine.tools.UFOProjectManager")
project = tool.ProjectManager()
project.source_files.append("main.cpp")
project.generate_project()

#build takes arguments
#_cmake_args - Arguments for cmake, such as debug flags like -ggdb to set optimisation level
# for debugging with GDB
# #_generator_option - For example -G Ninja if Ninja should be used. Can be left empty if using make
#_make_or_ninja - ninja if using ninja. make if using make.
#_make_or_ninja_args - -j6 for using additional cores to compile. Works with both ninja and make.
result = project.build("", "-G Ninja", "ninja" ,"-j6")

if not result: print("Build failed")
else: project.run()
```

Initialise submodules:
git submodule --init

Update submodules
git submodule update --recursive

Make sure you have a main.cpp file upon running project.py.
The minimum required code for the main file would be:
```c++
#include <ufo_engine.h>

int main(){

    Engine& ufoe = Engine::Get();

    ufoe.GoToLevel("../UFO-Engine/res/engine/default_level.json");
    ufoe.Start();

    return 0;
}
```

Your folder structure should now look something like this:

```
YOUR_GAME
|
|--/UFO-Engine
|
|--/build
|
|--/src
|
|--project.py
```