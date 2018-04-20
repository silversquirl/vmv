cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

project (vmv)
file(GLOB SOURCES "*.c")
add_executable(visualiser ${SOURCES})

find_package(soundio REQUIRED)
find_package(fftw 3 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Lua 5.3 REQUIRED)

include_directories(${SOUNDIO_INCLUDE_DIR} ${FFTW_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${LUA_INCLUDE_DIR})
target_link_libraries(visualiser ${SOUNDIO_LIBRARY} ${FFTW_LIBRARIES} glfw ${GLEW_LIBRARY} ${LUA_LIBRARY} ${OPENGL_LIBRARY} m)
