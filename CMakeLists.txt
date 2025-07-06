cmake_minimum_required(VERSION 3.14)
project(kx-converter LANGUAGES CXX)

# We will set the C++ standard to 17 to match the code.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Fetch Dependencies ---
include(FetchContent)
FetchContent_Declare(cli11 GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git GIT_TAG v2.3.2)
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.12.0)
FetchContent_Declare(nlohmann_json GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG v3.11.2)
FetchContent_Declare(indicators GIT_REPOSITORY https://github.com/p-ranav/indicators.git GIT_TAG v2.2)
FetchContent_MakeAvailable(cli11 spdlog nlohmann_json indicators)


# --- Core Library Target ---
add_library(kx_converter_lib
    src/converter.cpp
    src/models.cpp
    src/parser.cpp
)
target_include_directories(kx_converter_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(kx_converter_lib PUBLIC
    # Use the header-only version of spdlog to prevent its own linking conflicts.
    spdlog::spdlog_header_only
    nlohmann_json::nlohmann_json
    indicators::indicators
)

# --- Executable Target ---
add_executable(kx-converter app/main.cpp)
target_link_libraries(kx-converter PRIVATE
    kx_converter_lib
    CLI11::CLI11
)

if(WIN32)
    target_sources(kx-converter PRIVATE data/icon.rc)
endif()

# --- THE DEFINITIVE RUNTIME LIBRARY FIX ---
# We will now manually add the correct compiler flags to every single one of our targets.
# This is a low-level, direct command that cannot be overridden by IDE settings.
# /MT  = Static Release Runtime
# /MTd = Static Debug Runtime
if(MSVC)
  # Apply to the library
  target_compile_options(kx_converter_lib PRIVATE
      $<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT> $<$<CONFIG:RelWithDebInfo>:/MT> $<$<CONFIG:MinSizeRel>:/MT>
  )
  # Apply to the main executable
  target_compile_options(kx-converter PRIVATE
      $<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT> $<$<CONFIG:RelWithDebInfo>:/MT> $<$<CONFIG:MinSizeRel>:/MT>
  )
endif()
# --- END OF FIX ---


# --- Testing ---
option(BUILD_TESTING "Build the tests" ON)
if(BUILD_TESTING)
    enable_testing()

    FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(test) # This defines the parser_test target

    # --- APPLY THE SAME EXPLICIT FIX TO THE TEST EXECUTABLE ---
    # This is the step that was missing and caused the final error.
    if(MSVC)
      target_compile_options(parser_test PRIVATE
          $<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT> $<$<CONFIG:RelWithDebInfo>:/MT> $<$<CONFIG:MinSizeRel>:/MT>
      )
    endif()
endif()