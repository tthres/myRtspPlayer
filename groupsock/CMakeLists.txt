cmake_minimum_required(VERSION 3.5)

project(live555)

file(GLOB USER_LIBS_FILE ./*.cpp)
file(GLOB USER_LIBS_C_FILE ./*.c)

add_library(groupsock
        STATIC
        ${USER_LIBS_FILE}
        ${USER_LIBS_C_FILE})


if(WIN32)
        target_compile_options(groupsock PRIVATE /W4)
        message(STATUS "groupsock build in WIN32")
else()
        target_compile_options(groupsock PRIVATE -std=c++11 -O -DSOCKLEN_T=socklen_t -g -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 -Wall -DBSD=1)
        message(STATUS "groupsock build in UNIX")
endif()

target_compile_definitions(groupsock PRIVATE -DNO_GETIFADDRS)
message(STATUS "Added NO_GETIFADDRS definition to groupsock")
target_include_directories(groupsock
        PUBLIC .
        PUBLIC include
        PUBLIC ./../
        ../UsageEnvironment/include)
