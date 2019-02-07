cmake_minimum_required(VERSION 3.12)
project(hadcom)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

find_package(Qt5Widgets 5.12 REQUIRED)
find_package(Qt5Network 5.12 REQUIRED)

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

# Deployment
add_custom_target(Deploy)
set(INSTALL_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(DEPLOY_PATH ${CMAKE_SOURCE_DIR}/deploy/${CMAKE_BUILD_TYPE})
find_program(WINDEPLOYQT windeployqt)
add_custom_command(TARGET Deploy POST_BUILD COMMAND copy hadcom.exe \"${INSTALL_PATH}\" WORKING_DIRECTORY
        ${CMAKE_BINARY_DIR})
add_custom_command(TARGET Deploy POST_BUILD COMMAND ${WINDEPLOYQT} --no-quick-import --no-translations
        --no-system-d3d-compiler hadcom.exe WORKING_DIRECTORY ${INSTALL_PATH})
add_custom_command(TARGET Deploy POST_BUILD COMMAND cmake -E tar cf \"${DEPLOY_PATH}/HadCom-${VERSION_STRING}.zip\"
        --format=zip ${CMAKE_BUILD_TYPE} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
