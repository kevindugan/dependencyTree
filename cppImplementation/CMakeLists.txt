cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

project(DependencyGraph CXX)

enable_testing()
add_subdirectory(TPLs)
add_subdirectory(Graph)
