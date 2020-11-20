# CMakeList.txt : CMake project for opengl-stuff, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(opengl-stuff)

include_directories(include)

# Add source to this project's executable.
add_executable (opengl-stuff
	"src/glad.c"
	"src/opengl-stuff.cpp"
	"src/opengl-stuff.hpp")

target_link_libraries(opengl-stuff ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib)