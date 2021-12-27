set(GLRENDERER_RENDERER_BINARY glrenderer)

enable_language(C)
enable_language(CXX)
set(CMAKE_C_VERSION 99)
set(CMAKE_CXX_VERSION 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0075 NEW)


set(
  GLRENDERER_RENDERER_SOURCES
  matrix.cpp
  renderer.cpp
  loadobj.cpp
  Texture.cpp
  )

set(
  GLRENDERER_RENDERER_HEADERS
  glutil.hpp
  Bindable.hpp
  VertexArray.hpp
  IndexBuffer.hpp
  ConstantBuffer.hpp
  Drawable.hpp
  matrix.hpp
  renderer.hpp
  loadobj.hpp
  Texture.hpp
  )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(
  ${GLRENDERER_RENDERER_BINARY}
  ${GLRENDERER_RENDERER_SOURCES}
  ${GLRENDERER_RENDERER_HEADERS}
  )

target_compile_definitions(glrenderer PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

target_compile_features(glrenderer PRIVATE cxx_std_17)
target_compile_features(glrenderer PRIVATE c_std_99)

# Link SDL2 libs.
target_link_libraries(glrenderer PUBLIC ${SDL2_LIBRARY})
target_link_libraries(glrenderer PUBLIC fmt)
target_link_libraries(glrenderer PUBLIC GL)
target_link_libraries(glrenderer PUBLIC GLEW)

target_include_directories(glrenderer PUBLIC ${SDL2_INCLUDE_DIR})

if(${CMAKE_CXX_BYTE_ORDER} STREQUAL  "BIG_ENDIAN")
  target_compile_definitions(glrenderer PUBLIC GLPROJ_BIG_ENDIAN=1)
else()
  target_compile_definitions(glrenderer PUBLIC GLPROJ_LITTLE_ENDIAN=1)
endif()
