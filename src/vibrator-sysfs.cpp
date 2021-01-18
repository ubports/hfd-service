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

#include <iostream>
#include <unistd.h>

// if the kernel support led triggers
// we set 
namespace hfd {

bool VibratorSysfs::usable() {
    return access("/sys/class/leds/vibrator", F_OK ) != -1;
}

VibratorSysfs::VibratorSysfs(): Vibrator() {
    Udev::Udev udev;

    m_device = udev.device_from_syspath("/sys/class/leds/vibrator");

    try {
        m_device.set_sysattr("trigger", "transient");
    } catch (const std::runtime_error& e) {
        (void)e;
        std::cerr << "Failed to add transient trigger, usually non-fatal" << std::endl;
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
