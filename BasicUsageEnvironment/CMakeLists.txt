cmake_minimum_required(VERSION 3.5)

project(live555)

file(GLOB USER_LIBS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(BasicUsageEnvironment
    STATIC
    ${USER_LIBS_FILE}
)

# Specify compile options based on the platform
if(WIN32)
    target_compile_options(BasicUsageEnvironment PRIVATE /W4)
	message(STATUS "BasicUsageEnvironment build in WIN32")
else()
    target_compile_options(BasicUsageEnvironment PRIVATE -std=c++11 -O -DSOCKLEN_T=socklen_t -g -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 -Wall -DBSD=1)
	message(STATUS "BasicUsageEnvironment build in UNIX")
endif()

target_include_directories(BasicUsageEnvironment
    PUBLIC .
    PUBLIC include
    PUBLIC ../UsageEnvironment/include
    PUBLIC ../groupsock/include
)


