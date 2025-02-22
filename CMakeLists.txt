# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(whirlpool VERSION 0.1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find dependencies
find_package(nlohmann_json 3.2.0 REQUIRED)

# Add library (header-only)
add_library(whirlpool INTERFACE)
target_include_directories(whirlpool
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(whirlpool
  INTERFACE
    nlohmann_json::nlohmann_json
)

# Add solutions directory
add_subdirectory(solutions)
