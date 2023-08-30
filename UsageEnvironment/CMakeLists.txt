# This should be the first line of the CMakeLists.txt
cmake_minimum_required(VERSION 3.5)

project(live555)

# Make sure all source files are globbed
file(GLOB USER_LIBS_FILE ./*.cpp)

# Define your library
add_library(UsageEnvironment 
        STATIC
        ${USER_LIBS_FILE})

# Set C++ standard to C++11
target_compile_features(UsageEnvironment PRIVATE cxx_std_11)

# Add platform specific compile options
if(WIN32)
  target_compile_options(UsageEnvironment PRIVATE /W4)
  message(STATUS "UsageEnvironment build in WIN32")
elseif(UNIX)
target_compile_options(UsageEnvironment PRIVATE -std=c++11 -O -DSOCKLEN_T=socklen_t -g -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 -Wall -DBSD=1)
  message(STATUS "UsageEnvironment build in UNIX")
endif()

# Set optimization level
target_compile_options(UsageEnvironment PRIVATE -O)

# Include directories
target_include_directories(UsageEnvironment 
        PUBLIC .
        PUBLIC include
        PUBLIC ../groupsock/include)
