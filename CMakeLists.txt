cmake_minimum_required(VERSION 3.12)
project(hadcom)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

include_directories("include")

add_executable(hadcom src/main.cpp src/main_window.cpp include/main_window.h include/mode_chooser.h src/mode_chooser.cpp
        include/user_info.h include/server.h src/server.cpp include/connection.h src/connection.cpp include/local_client.h
        src/local_client.cpp include/packets/packet.h src/packets/packet.cpp include/packets/handshake_packet.h src/packets/handshake_packet.cpp include/packets/client_joined_packet.h src/packets/client_joined_packet.cpp src/packets/packet_factory.cpp include/packets/packet_factory.h include/remote_client.h src/remote_client.cpp include/packets/client_left_packet.h src/packets/client_left_packet.cpp)
target_link_libraries(hadcom Qt5::Widgets Qt5::Network)
