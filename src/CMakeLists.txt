add_subdirectory(udev)

include_directories(
    ${ANDROID_HEADERS_INCLUDE_DIRS}
)



add_library(hfd-core STATIC
    leds.cpp
    leds-sysfs.cpp
    leds-hybris.cpp
    leds-hidl.cpp

    vibrator.cpp
    vibrator-sysfs.cpp
    vibrator-legacy.cpp
    vibrator-hidl.cpp

    bridge/bridge_defs.h
    bridge/bridge.h
    bridge/hybris_bridge_defs.h

    hardware/hfd_api.cpp
    hardware/android_hw_module.h
)

target_link_libraries(hfd-core
    udev-cpp
    ${ANDROID_HARDWARE_LIBRARIES}
    ${Hybris}
)

set(SERVICE_SRC
    service.cpp
    dbusAdaptor.h
)

qt5_add_dbus_adaptor(SERVICE_SRC
    ${CMAKE_SOURCE_DIR}/data/com.lomiri.hfd.xml ${CMAKE_CURRENT_SOURCE_DIR}/dbusAdaptor.h DbusAdaptor
)

message("ff ${SERVICE_SRC}")

add_executable(hfd-service
    ${SERVICE_SRC} 
)

target_link_libraries(hfd-service
    hfd-core

    Qt5::Core
    Qt5::DBus
)

install(TARGETS hfd-service RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
