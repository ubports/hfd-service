find_package(PkgConfig)
find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Feedback REQUIRED)

qt5_add_resources(FEEDBACK_RESOURCES feedback.qrc)

set(QTFEEDBACK_HFD_SRC
  hfd_feedback.h
  hfd_feedback.cpp
)

qt5_add_dbus_interface(QTFEEDBACK_HFD_SRC
    ${CMAKE_SOURCE_DIR}/data/com.lomiri.hfd.xml hfdInterface
)

add_library(
  qtfeedback_hfd SHARED

  ${QTFEEDBACK_HFD_SRC}
  ${FEEDBACK_RESOURCES}
)

# Ideally, we would read the plugin installation location from cmake
# but this does not work currently.
set(PLUGIN_INSTALL_LOCATION "${CMAKE_INSTALL_LIBDIR}/qt5/plugins/feedback")
# get_target_property(PLUGIN_LOCATION Qt5::Feedback PLUGIN_LOCATION)

message(STATUS "Installing Qt5 feedback plugin to: ${PLUGIN_INSTALL_LOCATION}")

qt5_use_modules(qtfeedback_hfd Core DBus Feedback)

target_link_libraries(
  qtfeedback_hfd

  ${CMAKE_THREAD_LIBS_INIT}

  Qt5::Core
  Qt5::DBus
  Qt5::Feedback
)

install(
  TARGETS qtfeedback_hfd
  LIBRARY DESTINATION ${PLUGIN_INSTALL_LOCATION})
