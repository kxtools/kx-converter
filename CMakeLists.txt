cmake_minimum_required(VERSION 3.10)
project(kx-converter CXX)

if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
endif()

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


if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Enable AVX2
    LIST(APPEND CXX_FLAGS "/arch:AVX2")

    # https://docs.microsoft.com/en-us/cpp/build/reference/o-options-optimize-code?view=msvc-170
    # Enable optimization that shouldn't cause issues when debugging
    LIST(APPEND CXX_FLAGS "/Oy")

    # Enable optimization for release
    LIST(APPEND CXX_FLAGS_RELEASE "/O2")
endif()

target_compile_options(${PROJECT_NAME} PUBLIC ${CXX_FLAGS}
     "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>"
     "$<$<CONFIG:Release>:${CXX_FLAGS_RELEASE}>")


target_link_libraries(${PROJECT_NAME} spdlog)