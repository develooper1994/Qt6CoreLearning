cmake_minimum_required(VERSION 3.14)

project(Networking LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Concurrent Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Concurrent Network)

add_executable(Networking
  main.cpp
  udpclient.h udpclient.cpp

  commandLine.h commandLine.cpp
  chat.h chat.cpp

  tcpclient.h tcpclient.cpp
  networkaccessmanager.h networkaccessmanager.cpp

  tcpserver.h tcpserver.cpp

  multithreadsenkrontcpclient.h multithreadsenkrontcpclient.cpp
  multithreadtcpserver.h multithreadtcpserver.cpp
)
target_link_libraries(Networking Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Concurrent Qt${QT_VERSION_MAJOR}::Network)

install(TARGETS Networking
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
