file(GLOB_RECURSE "GLAD_SOURCES" RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
src/*.c
src/*.h
)

add_library(glad
	${GLAD_SOURCES}
)

target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)