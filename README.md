#for windows

cmake -G "Visual Studio 17 2022" -A Win32 -B "build"


cmake --build . --config Release
cmake --build . --config Debug

#glfw : https://github.com/glfw/glfw
#git remote add glfw https://github.com/glfw/glfw
#git subtree add --prefix=external/glfw glfw master --squash
#git subtree pull --prefix=external/glfw glfw master --squash


#glad: https://github.com/Dav1dde/glad