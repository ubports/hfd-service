find_package(QmlPlugins)
find_package(Qt5Gui REQUIRED)

set(HFD_QML_SRC
    plugin.cpp
    leds.cpp
)

qt5_add_dbus_interface(HFD_QML_SRC
    ${CMAKE_SOURCE_DIR}/data/com.lomiri.hfd.xml hfdInterface
)

add_library(hfd-qml MODULE
    ${HFD_QML_SRC}
)

target_link_libraries(hfd-qml
    hfd-core

    Qt5::Core
    Qt5::Gui
    Qt5::DBus
)

add_qmlplugin(
  Hfd
  1.0
  Hfd
  TARGET_PREFIX Hfd
  TARGETS hfd-qml
)
