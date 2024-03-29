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
  else()
    message(WARNING "Bulding without libhybris support, missing required dependencies!")
  endif()
else()
  message(STATUS "Bulding without libhybris support")
endif()

option(ENABLE_LIBGBINDER "Enable libgbinder support" ON)

if (ENABLE_LIBGBINDER)
  pkg_check_modules(GLIB_UTIL libglibutil)
  pkg_check_modules(GBINDER libgbinder)
  if(GLIB_UTIL_FOUND AND GBINDER_FOUND)
    message(STATUS "Bulding with libgbinder support")
    set(HAVE_LIBGBINDER true)
  else()
    message(WARNING "Bulding without libgbinder support, missing required dependencies!")
  endif()
else()
  message(STATUS "Bulding without libgbinder support")
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(qt)

# Dbus file
install(FILES data/hfd-service.conf 
  DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/init
)

# Dbus policy
install(FILES data/com.lomiri.hfd.conf
  DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR}/dbus-1/system.d
)

# Dbus service file
configure_file(data/com.lomiri.hfd.service.in com.lomiri.hfd.service)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/com.lomiri.hfd.service
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/system-services
)

