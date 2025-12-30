cmake_minimum_required(VERSION 3.16)
project(ccar_ola LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ccar_ola
  src/main.cpp
  src/distributions.cpp
  src/simulator.cpp
  src/aggregator.cpp
  src/io.cpp
)

target_include_directories(ccar_ola PRIVATE include)
