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
  InputMap.cpp
  renderer/renderer.cpp
  renderer/loadobj.cpp
  renderer/Texture.cpp
  renderer/Camera.cpp
  renderer/Shader.cpp
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
  InputMap.hpp
  renderer/glutil.hpp
  renderer/Bindable.hpp
  renderer/VertexArray.hpp
  renderer/IndexBuffer.hpp
  renderer/ConstantBuffer.hpp
  renderer/Drawable.hpp
  renderer/renderer.hpp
  renderer/loadobj.hpp
  renderer/Texture.hpp
  renderer/Shader.hpp
  )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(
  ${GLPROJECT_BINARY_FINAL}
  main.cpp
  ${GLPROJECT_SOURCES}
  ${GLPROJECT_HEADERS}
  )

target_compile_definitions(glproject PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_compile_features(glproject PRIVATE cxx_std_17)
target_compile_features(glproject PRIVATE c_std_99)

# Link libm
target_link_libraries(glproject PUBLIC m)
target_link_libraries(glproject PUBLIC glad::glad)
target_link_libraries(glproject PUBLIC OpenGL::GL)
target_link_libraries(glproject PUBLIC ${CMAKE_DL_LIBS})
target_link_libraries(glproject PUBLIC glm::glm)
target_link_libraries(glproject PUBLIC fmt::fmt)
# Link SDL2 libs.
target_link_libraries(glproject PUBLIC ${SDL2_LIBRARY})
target_link_libraries(glproject PUBLIC ${SDL2_MIXER_LIBRARIES})
target_link_libraries(glproject PUBLIC ${SDL2_TTF_LIBRARIES})

target_include_directories(glproject PUBLIC ${SDL2_INCLUDE_DIR})
target_include_directories(glproject PUBLIC ${SDL2_MIXER_INCLUDE_DIRS})
target_include_directories(glproject PUBLIC ${SDL2_TTF_INCLUDE_DIRS})
target_include_directories(glproject PUBLIC
  ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(glproject PUBLIC
  ${PROJECT_SOURCE_DIR}/external/glm)
target_include_directories(glproject PUBLIC
  ${PROJECT_SOURCE_DIR}/external/fmt/include/)


if(${CMAKE_CXX_BYTE_ORDER} STREQUAL  "BIG_ENDIAN")
  target_compile_definitions(glproject PUBLIC PROJ_BIG_ENDIAN=1)
else()
  target_compile_definitions(glproject PUBLIC PROJ_LITTLE_ENDIAN=1)
endif()

add_custom_target(run
    COMMAND glproject
    DEPENDS glproject
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
