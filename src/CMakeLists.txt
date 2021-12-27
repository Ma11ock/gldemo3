set(GLPROJECT_BINARY_FINAL glproject)

enable_language(C)
enable_language(CXX)
set(CMAKE_C_VERSION 99)
set(CMAKE_CXX_VERSION 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0075 NEW)


set(
  GLPROJECT_SOURCES
  util.cpp
  event.cpp
  frame.cpp
  gameLayer.cpp
  input.cpp
  graphics.cpp
  )

set(
  GLPROJECT_HEADERS
  util.hpp
  event.hpp
  frame.hpp
  gameLayer.hpp
  keyboardEvent.hpp
  mouseEvent.hpp
  input.hpp
  graphics.hpp
  )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(
  ${GLPROJECT_BINARY_FINAL}
  main.cpp
  ${GLPROJECT_SOURCES}
  ${GLPROJECT_HEADERS}
  )
add_subdirectory(renderer)

target_compile_definitions(glproject PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_compile_features(glproject PRIVATE cxx_std_17)
target_compile_features(glproject PRIVATE c_std_99)

# Link libm
target_link_libraries(glproject PUBLIC m)
target_link_libraries(glproject PUBLIC fmt)
# Link SDL2 libs.
target_link_libraries(glproject PUBLIC ${SDL2_LIBRARY})
target_link_libraries(glproject PUBLIC ${SDL2_MIXER_LIBRARIES})
target_link_libraries(glproject PUBLIC ${SDL2_TTF_LIBRARIES})
target_link_libraries(glproject PUBLIC ${SDL2_IMAGE_LIBRARIES})

target_include_directories(glproject PUBLIC ${SDL2_INCLUDE_DIR})
target_include_directories(glproject PUBLIC ${SDL2_MIXER_INCLUDE_DIRS})
target_include_directories(glproject PUBLIC ${SDL2_TTF_INCLUDE_DIRS})
target_include_directories(glproject PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(glproject PUBLIC glrenderer)

if(${CMAKE_CXX_BYTE_ORDER} STREQUAL  "BIG_ENDIAN")
  target_compile_definitions(glproject PUBLIC DOOM_BIG_ENDIAN=1)
else()
  target_compile_definitions(glproject PUBLIC DOOM_LITTLE_ENDIAN=1)
endif()

add_custom_target(run
    COMMAND glproject
    DEPENDS glproject
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
