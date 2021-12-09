cmake_minimum_required(VERSION 3.0.0)
project(VolumeRendering VERSION 0.1.0)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(LIBS )

find_package(glad CONFIG REQUIRED)
set(LIBS ${LIBS} glad::glad)

find_package(glfw3 CONFIG REQUIRED)
set(LIBS ${LIBS} glfw)

set(SOURCE "${CMAKE_SOURCE_DIR}/src/CS247_Assignment4.cpp")

set(EXEC "VolumeRendering")

add_executable(${EXEC} ${SOURCE})
target_link_libraries(${EXEC} PRIVATE ${LIBS})