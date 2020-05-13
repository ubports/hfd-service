cmake_minimum_required(VERSION 3.0)
project(hfd-service)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(FindPkgConfig)
include(GNUInstallDirs)

pkg_check_modules(UDEV REQUIRED libudev)

option(ENABLE_LIBHYBRIS "Enable libhybris support" ON)

if (ENABLE_LIBHYBRIS)
  pkg_check_modules(ANDROID_HEADERS android-headers)
  pkg_check_modules(ANDROID_HARDWARE libhardware)
  if(ANDROID_HEADERS_FOUND AND ANDROID_HARDWARE_FOUND)
    message(STATUS "Bulding with libhybris support")
    set(HAVE_LIBHYBRIS true)
    # Try to find hybris, and disable hybris from build if not found
    find_library(HYBRIS_COMMON_LIBRARY
      NAMES hybris-common
    )
  else()
    message(WARNING "Bulding without libhybris support, missing required dependencies!")
  endif()
else()
  message(STATUS "Bulding without libhybris support")
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)

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
