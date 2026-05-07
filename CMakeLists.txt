cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME quoka)
set(CMAKE_BUILD_TYPE Debug)
project(${PROJECT_NAME} LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE 
    src/info/qconsole_logger.cpp
    src/info/qlog_manager.cpp
    src/http/qserver.cpp
    src/http/qlistener.cpp
    src/http/qrouter.cpp
    src/http/dto/qrequest.cpp
    src/http/dto/qresponse.cpp
    src/utils/qconfig.cpp
    src/detail/qtcp_sokcet.cpp
    src/detail/qrequest_pool.cpp
    src/qexception.cpp
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE ${PROJECT_NAME})