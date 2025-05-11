cmake_minimum_required(VERSION 3.10)
project(waynotify)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
# DBUS
pkg_check_modules(DBUS REQUIRED dbus-1)
# SDL2
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
PKG_CHECK_MODULES(SDL2_IMAGE REQUIRED SDL2_image)

# Get the directories
include_directories(${DBUS_INCLUDE_DIRS} /usr/include/dbus-1.0 /usr/lib/dbus-1.0/include)
include_directories(/usr/include/dbus-1.0 /usr/lib/x86_64-linux-gnu/dbus-1.0/include)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

link_directories(${DBUS_LIBRARY_DIRS})
link_directories(${SDL2_LIBRARY_DIRS} ${SDL2_TTF_LIBRARY_DIRS} ${SDL2_IMAGE_LIBRARY_DIRS})


add_executable(waynotify
    src/main.cpp
    src/dbus_listener.cpp
    src/notification_handler.cpp
    src/sdl_render.cpp
)

target_link_libraries(waynotify ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${DBUS_LIBRARIES} )
