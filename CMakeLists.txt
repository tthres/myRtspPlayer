#This should be the first line of the CMakeLists.txt
cmake_minimum_required(VERSION 3.5)

project(live555)

add_subdirectory(groupsock)
add_subdirectory(BasicUsageEnvironment)
add_subdirectory(UsageEnvironment)
add_subdirectory(liveMedia)

# Specify compile options based on the platform
if(WIN32)
        add_compile_options(/W4)
        message(STATUS "testRTSPClient build in WIN32")
else()
        add_compile_options(-std=c++11 -O -DSOCKLEN_T=socklen_t -g -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 -Wall -DBSD=1)
        message(STATUS "testRTSPClient build in UNIX")
endif()

add_definitions(-DNO_OPENSSL)

# test RTSPClient
file(GLOB RTSPClient_SRC "RTSPClient/*.cpp" "RTSPClient/*.h") 
add_executable(testRTSPClient ${RTSPClient_SRC})    

# FFmpeg include directory
set(FFMPEG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/packages/FFmpeg.Nightly.20190422.0.0/include)
set(FFMPEG_LIB_DIR ${CMAKE_SOURCE_DIR}/packages/FFmpeg.Nightly.20190422.0.0/lib-Win32)


add_library( avcodec STATIC IMPORTED)
add_library( avfilter STATIC IMPORTED )
add_library( swresample STATIC IMPORTED )
add_library( swscale STATIC IMPORTED )
add_library( avformat STATIC IMPORTED )
add_library( avutil STATIC IMPORTED )


#指定所添加依赖库的导入路径
set_target_properties( avcodec PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/avcodec.lib )
set_target_properties( avfilter PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/avfilter.lib )
set_target_properties( swresample PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/swresample.lib )
set_target_properties( swscale PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/swscale.lib )
set_target_properties( avformat PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/avformat.lib )
set_target_properties( avutil PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/avutil.lib )

# 添加头文件路径到编译器的头文件搜索路径下，多个路径以空格分隔
include_directories( ${FFMPEG_INCLUDE_DIR} )
link_directories(${FFMPEG_LIB_DIR} )



target_link_libraries(testRTSPClient
        PRIVATE liveMedia
        PRIVATE groupsock
        PRIVATE BasicUsageEnvironment
        PRIVATE UsageEnvironment

        # D3D11 libraries
        PRIVATE dxgi
        PRIVATE d3d9

        # FFmpeg libraries
        PRIVATE avcodec
        PRIVATE avformat
        PRIVATE avutil
        PRIVATE swresample
        PRIVATE swscale
        )
target_include_directories(testRTSPClient 
        PUBLIC .
        PUBLIC include
        PUBLIC UsageEnvironment/include
        PUBLIC groupsock/include
        PUBLIC liveMedia/include
        PUBLIC BasicUsageEnvironment/include)

set(FFMPEG_DLLS
        ${FFMPEG_LIB_DIR}/avcodec-58.dll
        ${FFMPEG_LIB_DIR}/swresample-3.dll
        ${FFMPEG_LIB_DIR}/swscale-5.dll
        ${FFMPEG_LIB_DIR}/avutil-56.dll)

add_custom_command(TARGET testRTSPClient POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:testRTSPClient>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${FFMPEG_DLLS}
            $<TARGET_FILE_DIR:testRTSPClient>
        COMMENT "Copying FFmpeg DLLs to output directory")