find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
if(UNIX)
	find_package(PNG REQUIRED)
endif(UNIX)

file(GLOB_RECURSE "${PROJECT_NAME}_SOURCES" RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
${CMAKE_CURRENT_SOURCE_DIR}/*.cu
${CMAKE_CURRENT_SOURCE_DIR}/*.h
${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)
MESSAGE(STATUS "${PROJECT_NAME}_SOURCES: ${${PROJECT_NAME}_SOURCES}")

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_precompile_headers(${PROJECT_NAME} PUBLIC pch.h)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
if(UNIX)
	target_link_libraries(${PROJECT_NAME} PNG::PNG)
endif(UNIX)
if(WIN32)
	target_include_directories(${PROJECT_NAME} PRIVATE Vendor/libpng/include)
	target_link_directories(${PROJECT_NAME} PRIVATE Vendor/libpng/lib)
	target_link_libraries(${PROJECT_NAME} "zlibstat$<$<CONFIG:Debug>:d>.lib" "libpng$<$<CONFIG:Debug>:d>.lib")
endif(WIN32)

target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} imgui)

if(WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS} /SUBSYSTEM:WINDOWS")
endif(WIN32)
if(UNIX)
	target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
endif(UNIX)


if (CMAKE_CXX_COMPILER_ID MATCHES "(Clang|GNU)")
	target_compile_options(${PROJECT_NAME} PRIVATE
	"$<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic;-Wconversion;-fno-exceptions;$<$<CONFIG:RELEASE>:-O3;-ffast-math;-fno-rtti>>")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(${PROJECT_NAME} PRIVATE
		"$<$<COMPILE_LANGUAGE:CXX>:/Wall;/MP;/Oi;/GL;$<$<CONFIG:RELEASE>:/Ot;/fp:fast;/GR->>")
	set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY
		"MultiThreaded$<$<CONFIG:Debug>:Debug>")
	set_target_properties(glad PROPERTIES MSVC_RUNTIME_LIBRARY
		"MultiThreaded$<$<CONFIG:Debug>:Debug>")
	set_target_properties(glfw PROPERTIES MSVC_RUNTIME_LIBRARY
		"MultiThreaded$<$<CONFIG:Debug>:Debug>")
	set_target_properties(imgui PROPERTIES MSVC_RUNTIME_LIBRARY
		"MultiThreaded$<$<CONFIG:Debug>:Debug>")

	# Disable exceptions: https://cmake.org/pipermail/cmake/2010-December/041641.html
	if(CMAKE_CXX_FLAGS MATCHES "/EHsc ")
		string(REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	endif(CMAKE_CXX_FLAGS MATCHES "/EHsc ")

	if(CMAKE_CXX_FLAGS MATCHES "/EHsc$")
		string(REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	endif(CMAKE_CXX_FLAGS MATCHES "/EHsc$")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})