cmake_minimum_required(VERSION 3.5)

project(live555)

file(GLOB USER_LIBS_FILE ./*.cpp)
file(GLOB USER_LIBS_C_FILE ./*.c)

add_library(liveMedia
        STATIC
        ${USER_LIBS_FILE}
        ${USER_LIBS_C_FILE})


if(WIN32)
        target_compile_options(liveMedia PRIVATE /W4)
        target_link_libraries(liveMedia
                PUBLIC Ws2_32
        )
        message(STATUS "liveMedia build in WIN32")
else()
        target_compile_options(liveMedia PRIVATE -std=c++11 -O -DSOCKLEN_T=socklen_t -g -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 -Wall -DBSD=1)
        message(STATUS "liveMedia build in UNIX")
endif()

add_definitions(-DNO_OPENSSL)
message(STATUS "Added NO_OPENSSL definition to liveMedia")

target_compile_definitions(liveMedia PUBLIC -DNO_GETIFADDRS=1)

target_include_directories(liveMedia
        PUBLIC .
        PUBLIC include
        PUBLIC ../UsageEnvironment/include
        PUBLIC ../groupsock/include)
