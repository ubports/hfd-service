/*
 * Copyright 2020 UBports foundation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Marius Gripsgard <marius@ubports.com>
 */

#include "vibrator-sysfs.h"

#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h>

// if the kernel support led triggers
// we set 
namespace hfd {

// True if and only if the standard transient trigger is supported.
// Some devices implement the transient trigger but don't bother
// Actually support it, this function will return a false negative
// in that case
bool VibratorSysfs::supportTransient() {
    std::ifstream source_stream("/sys/class/leds/vibrator/trigger");

    if (!source_stream) {
        std::cerr << "Can't read '/sys/class/leds/vibrator/trigger', does the file exist?" << std::endl;
        return false;
    }

    std::string supported_triggers = std::string(std::istreambuf_iterator<char>(source_stream),
                                std::istreambuf_iterator<char>());
    
    bool transientSupported = supported_triggers.find("transient") != std::string::npos;
    std::cout << "Vibrator supports transient trigger: " << transientSupported << std::endl;

    return transientSupported;
}

// Checks that the transient trigger is supported,
// or implemented manually by the driver
bool VibratorSysfs::usable() {
    // Need to be able to access the vibrator
    bool usable = (access("/sys/class/leds/vibrator", F_OK ) != -1);
    if (!usable) {
        return false;
    }

    // Check if transient is in the list of supported triggers
    usable = (supportTransient());

    // Check if the device supports the transient trigger implicitly.
    usable |= (access("/sys/class/leds/vibrator/duration", F_OK ) != -1
            && access("/sys/class/leds/vibrator/state", F_OK ) != -1
            && access("/sys/class/leds/vibrator/activate", F_OK ) != -1);

    std::cout << "Usable: " << usable << std::endl;

    return usable;
}

VibratorSysfs::VibratorSysfs(): Vibrator() {
    Udev::Udev udev;

    m_device = udev.device_from_syspath("/sys/class/leds/vibrator");

    // If we reached this point but supportTransient() returns false
    // then the device implements the transient trigger implicitly
    // so we don't need to enable the transient trigger
    if (supportTransient()) {
        m_device.set_sysattr("trigger", "transient");
    }

    // Make sure we are off on init
    configure(State::Off, 0);
}

// Over simplyfied led controlls!
void VibratorSysfs::configure(State state, int durationMs) {
    if (state == State::On) {
        m_device.set_sysattr("state", "1");
        m_device.set_sysattr("duration", std::to_string(durationMs));
        m_device.set_sysattr("activate", "1");
    } else {
        m_device.set_sysattr("activate", "0");
    }
}
}
