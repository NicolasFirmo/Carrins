file(GLOB_RECURSE "IMGUI_SOURCES" RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(imgui ${IMGUI_SOURCES})

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui glad)
target_link_libraries(imgui glfw)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)