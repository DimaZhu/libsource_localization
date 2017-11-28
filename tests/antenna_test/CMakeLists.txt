cmake_minimum_required(VERSION 2.8.12)

project(antenna_test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

add_library(antenna SHARED ${CMAKE_CURRENT_SOURCE_DIR}/../../source_localization/antenna.cpp)
target_link_libraries(antenna Qt5::Core)


add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} Qt5::Core antenna)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../source_localization/)
