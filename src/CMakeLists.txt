add_subdirectory(udev)

if (HAVE_LIBHYBRIS)
    include_directories(
        ${ANDROID_HEADERS_INCLUDE_DIRS}
    )

    set(LIBHYBRIS_SRC
        leds-hybris.cpp
    )

    set(LIBHYBRIS_LIBRARIES
        ${ANDROID_HARDWARE_LIBRARIES}
    )

    add_definitions(-DHAVE_LIBHYBRIS)
endif()

if (HAVE_LIBGBINDER)
    include_directories(
        ${GLIB_UTIL_INCLUDE_DIRS}
        ${GBINDER_INCLUDE_DIRS}
    )

    set(LIBGBINDER_SRC
        leds-binder.cpp
        vibrator-binder.cpp
    )

    set(LIBGBINDER_LIBRARIES
        ${GLIB_UTIL_LDFLAGS} ${GLIB_UTIL_LIBRARIES}
        ${GBINDER_LDFLAGS} ${GBINDER_LIBRARIES}
    )

    add_definitions(-DHAVE_LIBGBINDER)
endif()

add_library(hfd-core STATIC
    leds.cpp
    leds-sysfs.cpp

    vibrator.cpp
    vibrator-ff.cpp
    vibrator-sysfs.cpp
    vibrator-legacy.cpp

    ${LIBHYBRIS_SRC}
    ${LIBGBINDER_SRC}
)

target_link_libraries(hfd-core
    udev-cpp
    ${LIBHYBRIS_LIBRARIES}
    ${LIBGBINDER_LIBRARIES}
)

set(SERVICE_SRC
    service.cpp
    dbusAdaptor.h
)

qt5_add_dbus_adaptor(SERVICE_SRC
    ${CMAKE_SOURCE_DIR}/data/com.lomiri.hfd.xml ${CMAKE_CURRENT_SOURCE_DIR}/dbusAdaptor.h DbusAdaptor
)

add_executable(hfd-service
    ${SERVICE_SRC} 
)

target_link_libraries(hfd-service
    hfd-core

    Qt5::Core
    Qt5::DBus
)

install(TARGETS hfd-service RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
