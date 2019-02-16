cmake_minimum_required(VERSION 3.12)
project(hadcom)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

find_package(Qt5Widgets 5.12 REQUIRED)
find_package(Qt5Network 5.12 REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

if(${CMAKE_BUILD_TYPE} EQUAL Release)
    add_compile_definitions(QT_NO_DEBUG_OUTPUT)
endif()

file(GLOB_RECURSE SOURCE "src/*.cpp")
file(GLOB_RECURSE INCLUDE "include/*.h")

add_executable(hadcom ${SOURCE} ${INCLUDE})
target_include_directories(hadcom PRIVATE "include")
target_link_libraries(hadcom Qt5::Widgets Qt5::Network)

# Semantic versioning
execute_process(COMMAND git describe --tags OUTPUT_VARIABLE VERSION_STRING)
string(STRIP ${VERSION_STRING} VERSION_STRING)
message("Deploying v.${VERSION_STRING}")
configure_file(include/version.h.in ../include/version.h @ONLY)
