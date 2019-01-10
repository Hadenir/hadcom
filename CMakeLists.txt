cmake_minimum_required(VERSION 3.12)
project(hadcom)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

include_directories("include")

file(GLOB_RECURSE SOURCE "src/*.cpp")
file(GLOB_RECURSE INCLUDE "include/*.h")

add_executable(hadcom ${SOURCE} ${INCLUDE})
target_link_libraries(hadcom Qt5::Widgets Qt5::Network)
