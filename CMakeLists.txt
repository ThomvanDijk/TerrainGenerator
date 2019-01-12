# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (Terrain_Generator)

# Compile external dependencies.
find_package(OpenGL REQUIRED)

# Compile external dependencies 
add_subdirectory(external)

include_directories(
	${CMAKE_SOURCE_DIR}/external/glfw-3.0.3/include/GLFW/
	${CMAKE_SOURCE_DIR}/external/glm-0.9.8.4
	${CMAKE_SOURCE_DIR}/external/glew-1.9.0/include/
)

link_directories(
	${CMAKE_SOURCE_DIR}/external/glfw-3.0.3/src/
	${CMAKE_SOURCE_DIR}/external/glew-1.9.0/
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLFW_303
	GLEW_190
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# Program files.
add_executable(program
	program/main.cpp
	program/shader.cpp
	program/shader.h
)
target_link_libraries(program
	${ALL_LIBS}
)
file(
	COPY program/shaders
	DESTINATION ${CMAKE_BINARY_DIR}
)