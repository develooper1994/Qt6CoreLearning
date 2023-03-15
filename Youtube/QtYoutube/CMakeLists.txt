cmake_minimum_required(VERSION 3.14)

project(QtYoutube LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Concurrent Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Concurrent Network)

add_executable(QtYoutube
  main.cpp

  udpclient.h udpclient.cpp

  commandline.h commandline.cpp
  chat.h chat.cpp

  tcpclient.h tcpclient.cpp
)
target_link_libraries(QtYoutube Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Concurrent Qt${QT_VERSION_MAJOR}::Network)

install(TARGETS QtYoutube
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
