cmake_minimum_required(VERSION 3.10)
project(waynotify)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)

include_directories(${DBUS_INCLUDE_DIRS} /usr/include/dbus-1.0 /usr/lib/dbus-1.0/include)
include_directories(/usr/include/dbus-1.0 /usr/lib/x86_64-linux-gnu/dbus-1.0/include)

link_directories(${DBUS_LIBRARY_DIRS})

add_executable(waynotify
    src/main.cpp
    src/dbus_listener.cpp
    src/dbus_listener.hpp
)

target_link_libraries(waynotify ${DBUS_LIBRARIES})
