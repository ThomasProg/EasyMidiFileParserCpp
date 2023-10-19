cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

FILE(GLOB SOURCES Private/* Public/*)
set(LibName "EasyMidiFileParserCpp")
project(${LibName})

# Create library
add_library(${LibName} SHARED ${SOURCES})

# target_compile_definitions(${LibName} PRIVATE MMG_EXPORT)
target_compile_definitions(${LibName} PRIVATE MAKE_DLL)

target_include_directories(${LibName} PUBLIC 
    "${PROJECT_SOURCE_DIR}/Public/" 
    "${ModulesDirectory}/BaseMIDIHelpers/Public"
)

add_subdirectory(Tests)