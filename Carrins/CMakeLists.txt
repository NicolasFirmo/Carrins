find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(Vendor/glfw)
add_subdirectory(Vendor/glad)
add_subdirectory(Vendor/imgui)

file(GLOB_RECURSE "${PROJECT_NAME}_SOURCES" RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
src/*.cpp
src/*.h
)
MESSAGE(STATUS "${PROJECT_NAME}_SOURCES: ${${PROJECT_NAME}_SOURCES}")

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_precompile_headers(${PROJECT_NAME} PUBLIC src/pch.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

if(WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS } /SUBSYSTEM:WINDOWS")
endif(WIN32)
if(UNIX)
	target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
endif(UNIX)

target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} imgui)