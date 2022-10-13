cmake_minimum_required(VERSION 3.10)
project(kx-posrouter CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

file(GLOB_RECURSE SOURCE "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_subdirectory(libs/spdlog)
find_package(spdlog)

set(other_files
	libs/nlohmann/json.hpp
)

add_executable(${PROJECT_NAME} ${SOURCE} ${other_files})

target_link_libraries(${PROJECT_NAME} spdlog)