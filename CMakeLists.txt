cmake_minimum_required(VERSION 3.20)

# Calls on windows should look something like this:
# cmake .. -DCMAKE_BUILD_TYPE=debug
# "-DSDL2_MIXER_INCLUDE_DIR=C:\Program Files\SDL2_mixer-2.0.4\x86_64-w64-mingw32\include"
# "-DSDL2_MIXER_LIBRARY=C:\Program Files\SDL2_mixer-2.0.4\x86_64-w64-mingw32\lib\libSDL2_mixer.a"
# "-DSDL2_INCLUDE_DIR=C:\Program Files\SDL2-2.0.18\x86_64-w64-mingw32\include"
# "-DSDL2_LIBRARY=C:\Program Files\SDL2-2.0.18\x86_64-w64-mingw32\lib\libSDL2.a"
# "-DSDL2MAIN_LIBRARY=C:\Program Files\SDL2-2.0.18\x86_64-w64-mingw32\lib\libSDL2main.a"
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE release)
  message("Setting build type to release.")
else()
  set(CMAKE_BUILD_TYPE debug)
  set(GLPROJECT_TESTS 1)
  message("Build type is set to debug.")
endif()

project(glproject VERSION 0.0.1)

if(WIN32)
  set(EXTRA_VARS WIN32)
endif()

find_package(OpenGL REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/fmt)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/SDL)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/SDL_mixer)

add_subdirectory(src)

