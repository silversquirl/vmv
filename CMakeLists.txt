cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

project (vmv)
file(GLOB SOURCES "*.c")
add_executable(visualiser ${SOURCES})

add_subdirectory(extern/libsoundio)
add_subdirectory(extern/glfw)
add_subdirectory(extern/Lua)

find_package(FFTW 3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${FFTW_INCLUDE_DIR})
target_link_libraries(visualiser soundio ${FFTW_LIBRARIES} glfw lua_static GL m)
