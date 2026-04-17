cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME quoka_http)
project(${PROJECT_NAME} LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE 
    src/info/QConsoleLogger.cpp
    src/http/QServer.cpp
    src/QException.cpp
    src/net/QTcpSocket.cpp
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE ${PROJECT_NAME})