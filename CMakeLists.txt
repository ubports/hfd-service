cmake_minimum_required(VERSION 3.0)
project(hfd-service)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(FindPkgConfig)
include(GNUInstallDirs)

pkg_check_modules(UDEV REQUIRED libudev)
pkg_check_modules(ANDROID_HEADERS REQUIRED android-headers)
pkg_check_modules(ANDROID_HARDWARE REQUIRED libhardware)
find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)

# Try to find hybris, and disable hybris from build if not found
find_library(Hybris
    NAMES hybris-common
)

if(NOT Hybris)
    message(STATUS "Notice: libhybris-common.so not found, disabling hybris support")
    set(ENABLE_HYBRIS_IMPLEMENTATION OFF)
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(
    include
    src
)

add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(qt)

# Dbus file
install(FILES data/hfd-service.conf 
    DESTINATION /etc/init
)

# Dbus policy
install(FILES data/com.lomiri.hfd.conf
  DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/dbus-1/system.d
)
