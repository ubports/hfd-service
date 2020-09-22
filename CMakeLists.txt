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

find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(qt)

FILE(GLOB upstart_file "${CMAKE_CURRENT_SOURCE_DIR}/data/hfd-service.conf")
FILE(GLOB systemd_file "${CMAKE_CURRENT_SOURCE_DIR}/data/hfd-service.service")
FILE(GLOB dbus_file "${CMAKE_CURRENT_SOURCE_DIR}/data/com.lomiri.hfd.conf")

# Upstart file
install(FILES ${upstart_file}
    DESTINATION /etc/init
)

# Systemd file
pkg_check_modules(SYSTEMD systemd)
if (${SYSTEMD_FOUND})
  pkg_get_variable(SYSTEMD_SYSTEM_DIR systemd systemdsystemunitdir)
  message (STATUS "${SYSTEMD_SYSTEM_DIR} is the systemd system unit file install dir, installing ${systemd_file} to it")
  install (FILES "${systemd_file}"
           DESTINATION "${SYSTEMD_SYSTEM_DIR}")
else()
   message(WARNING "Did not find systmed, not installing systemd service")
endif()

# Dbus policy
install(FILES ${dbus_file}
  DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/dbus-1/system.d
)
