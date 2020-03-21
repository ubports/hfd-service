
add_library(udev-cpp STATIC 
    udev.cpp
    udevDevice.cpp
    udevEnumerate.cpp
    udevMonitor.cpp
)

target_link_libraries(udev-cpp 
    ${UDEV_LIBRARIES}
)
