add_executable(hfd-service-tools-leds
    hfd_leds.cpp
)
add_executable(hfd-service-tools-vibrator
    hfd_vibrator.cpp
)

include_directories("../src")

target_link_libraries(hfd-service-tools-leds
    hfd-core
)

target_link_libraries(hfd-service-tools-vibrator
    hfd-core
)

install(TARGETS hfd-service-tools-leds RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS hfd-service-tools-vibrator RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
