cmake_minimum_required(VERSION 3.0.0)
project(ADS_Client VERSION 0.1.0)

set(ADSLIB_SRC_DIR src/AdsLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
    message("@@@@@@" MSVC)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
endif()
# file(GLOB ADSLIB_SRC ${ADSLIB_SRC_DIR}/*.cpp ${ADSLIB_SRC_DIR}/*.h)
# add_library(AdsLib STATIC ${ADSLIB_SRC})

set(LIBS AdsLib)
include_directories(${ADSLIB_SRC_DIR})
link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

if(WIN32)
    set(AdsLib_LIBS ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/AdsLib.lib)
else()
    set(AdsLib_LIBS ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/AdsLib.a)
endif()

if(NOT EXISTS ${AdsLib_LIBS})
    message("Compiling ADS Library")
    add_subdirectory(src/AdsLib)
endif()

if(WIN32)
    list(APPEND LIBS ws2_32)
else()
    list(APPEND LIBS pthread)
endif()
 
add_executable(ADS_Client main.cpp)
target_link_libraries(ADS_Client ${LIBS})