cmake_minimum_required(VERSION 3.20)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE release)
  message("Setting build type to release.")
else()
  set(CMAKE_BUILD_TYPE debug)
  set(GLPROJECT_TESTS 1)
  message("Build type is set to debug.")
endif()

project(glproject VERSION 0.0.1)

find_package(SDL2)

if(NOT SDL2_FOUND OR NOT SDL2MAIN_LIBRARY)
  message(FATAL_ERROR "Could not find system SDL library")
endif()

find_package(SDL2_image)

if(NOT SDL2_IMAGE_FOUND)
  message(FATAL_ERROR "Could not find system SDL_image")
endif()

find_package(SDL2_ttf)

if(NOT SDL2_TTF_FOUND)
  message(FATAL_ERROR "Could not find system SDL_ttf")
endif()

find_package(SDL2_mixer)

if(NOT SDL2_MIXER_FOUND)
  message(FATAL_ERROR "Could not find system SDL_ttf")
endif()

find_package(OpenGL REQUIRED)

if(NOT OPENGL_FOUND)
  message(FATAL_ERROR "Could not find OpenGL")
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/external/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/fmt)

add_subdirectory(src)

