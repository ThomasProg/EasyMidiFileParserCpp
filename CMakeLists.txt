cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

# Set compiler flags to enable C++ exceptions
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
elseif(MSVC)
    # Add any necessary flags for MSVC
    # For MSVC, exceptions are enabled by default
endif()

FILE(GLOB SOURCES Private/* Public/* Private/*/* Public/*/*)
project(EasyMidiFileParserCpp)

# Create library
add_library(EasyMidiFileParserCpp SHARED ${SOURCES})

target_compile_definitions(EasyMidiFileParserCpp PRIVATE MAKE_DLL)

target_include_directories(EasyMidiFileParserCpp PUBLIC 
    "${PROJECT_SOURCE_DIR}/Public/" 
)

install(TARGETS EasyMidiFileParserCpp
        LIBRARY DESTINATION bin
        RUNTIME DESTINATION bin)

add_subdirectory(Tests)