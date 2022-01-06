cmake_minimum_required(VERSION 3.20)

project(glad)

add_library(glad
  STATIC
  glad.h
  glad.c
  )
add_library(glad::glad ALIAS glad)

target_compile_features(glad PRIVATE cxx_std_17)
target_compile_features(glad PRIVATE c_std_99)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
