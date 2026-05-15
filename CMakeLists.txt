cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME quoka)
set(CMAKE_BUILD_TYPE Debug)
project(${PROJECT_NAME} LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE 
    src/qconsole_logger.cpp
    src/qlog_manager.cpp
    src/qserver.cpp
    src/qlistener.cpp
    src/qrouter.cpp
    src/qrequest.cpp
    src/qresponse.cpp
    src/qconfig.cpp
    src/qtcp_sokcet.cpp
    src/qrequest_pool.cpp
    src/qexception.cpp
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)