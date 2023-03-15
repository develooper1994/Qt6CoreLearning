cmake_minimum_required(VERSION 3.14)

project(SignalSlot1-Radio-Station LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(SignalSlot1-Radio-Station
  main.cpp
  radio.h
  radio.cpp
  station.h
  station.cpp
)
target_link_libraries(SignalSlot1-Radio-Station Qt${QT_VERSION_MAJOR}::Core)

install(TARGETS SignalSlot1-Radio-Station
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
