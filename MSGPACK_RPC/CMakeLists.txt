cmake_minimum_required(VERSION 3.0.0)
project(calculator)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
set(RPCLIB_INCLUDE_DIR include)
set(RPCLIB_LIBS         rpc.lib)
set(RPCLIB_COMPILE_DEFINITIONS
    "ASIO_STANDALONE"
    "RPCLIB_ASIO=clmdep_asio"
    "RPCLIB_FMT=clmdep_fmt"
    "RPCLIB_MSGPACK=clmdep_msgpack"
)
set(RPCLIB_EXTRA_FLAGS "/EHsc")
set(RPCLIB_EXTRA_FLAGS_DEBUG "/Zi")

link_directories(.)
include_directories(${RPCLIB_INCLUDE_DIR})

add_executable(calculator_server calculator_server.cc)
target_link_libraries(calculator_server ${RPCLIB_LIBS})
set_target_properties(
        calculator_server
        PROPERTIES
        CXX_STANDARD 11
        COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${RPCLIB_EXTRA_FLAGS}")
target_compile_definitions(calculator_server PUBLIC ${RPCLIB_COMPILE_DEFINITIONS})

add_executable(calculator_client calculator_client.cc)
target_link_libraries(calculator_client ${RPCLIB_LIBS})
set_target_properties(
        calculator_client
        PROPERTIES
        CXX_STANDARD 14
        COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${RPCLIB_EXTRA_FLAGS}")
target_compile_definitions(calculator_client PUBLIC ${RPCLIB_COMPILE_DEFINITIONS})


