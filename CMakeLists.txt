# Based on the official SFML CMake template: https://github.com/SFML/cmake-sfml-project
cmake_minimum_required(VERSION 3.28)
project(SFMLSandbox LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

add_executable(SFMLSandbox src/main.cpp)
target_compile_features(SFMLSandbox PRIVATE cxx_std_20)
target_link_libraries(SFMLSandbox PRIVATE SFML::Graphics)

if(MSVC)
    target_compile_options(SFMLSandbox PRIVATE /W4 /permissive-)
endif()

# Copy assets next to the executable so "assets/..." resolves when run from the output folder.
add_custom_command(TARGET SFMLSandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SFMLSandbox>/assets)

# Run/debug from the executable's folder in Visual Studio.
set_target_properties(SFMLSandbox PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:SFMLSandbox>)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SFMLSandbox)
