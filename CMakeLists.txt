cmake_minimum_required(VERSION 3.1)

project(a1)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=gnu++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-variable")


set(A1_LIBS ${OPENGL_gl_LIBRARY})

# GLFW
set(GLFW_INSTALL OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)

add_subdirectory(glfw)
list(APPEND A1_LIBS glfw)
list(APPEND A1_INCLUDES glfw/include)

add_definitions(-DGLEW_STATIC)
list(APPEND A1_INCLUDES glew/include)
list(APPEND A1_SRC glew/src/glew.c)
source_group(GLEW FILES glew/src/glew.c)

# vecmath include directory
include_directories(vecmath/include)
add_subdirectory(vecmath)
list(APPEND A1_LIBS vecmath)
list(APPEND A1_INCLUDES vecmath/include)
list(
  APPEND
  A1_SRC
  src/main.cpp
  src/camera.cpp
  src/curve.cpp
  src/parse.cpp
  src/surf.cpp
  src/vertexrecorder.cpp
  src/Utils.cpp)
list(
  APPEND
  A1_HEADER
  src/camera.h
  src/curve.h
  src/parse.h
  src/vertexrecorder.h
  src/surf.h
  src/Utils.h)

add_executable(a1 ${A1_SRC} ${A1_HEADER})
target_include_directories(a1 PUBLIC ${A1_INCLUDES})
target_link_libraries(a1 ${A1_LIBS})

add_custom_command(
  TARGET a1
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/shaders
          ${CMAKE_BINARY_DIR}/shaders
  COMMENT "Copying shaders"
  VERBATIM)
