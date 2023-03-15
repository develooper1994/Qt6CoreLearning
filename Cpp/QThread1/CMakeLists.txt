cmake_minimum_required(VERSION 3.14)

project(QThread1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(QThread1
  main.cpp
  test.h test.cpp
  worker.h worker.cpp
  manager.h manager.cpp
  counter.h counter.cpp
  workersemaphores.h workersemaphores.cpp
  producer.h producer.cpp
  consumer.h consumer.cpp
  widget.h widget.cpp
)
target_link_libraries(QThread1 Qt${QT_VERSION_MAJOR}::Core)

install(TARGETS QThread1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
