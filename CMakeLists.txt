cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CUDA_COMPILER $ENV{CUDA_DIR})

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

project(Carrins LANGUAGES C CXX CUDA)

add_subdirectory(vendor/glad)
 
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
if(NOT BUILD_SHARED_LIBS)
	set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
endif(NOT BUILD_SHARED_LIBS)
add_subdirectory(vendor/glfw)

add_subdirectory(vendor/glm)

add_subdirectory(vendor/imgui)

add_subdirectory(src)