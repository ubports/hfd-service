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

#include "vibrator-legacy.h"

#include <iostream>
#include <unistd.h>

// if the kernel support led triggers
// we set 
namespace hfd {

bool VibratorLegacy::usable() {
    return access("/sys/class/timed_output/vibrator", F_OK ) != -1;
}

VibratorLegacy::VibratorLegacy(): Vibrator() {
    Udev::Udev udev;
    m_device = udev.device_from_syspath("/sys/class/timed_output/vibrator");

    // Make sure we are off on init
    configure(State::Off, 0);
}

VibratorLegacy::~VibratorLegacy()
{
}

// Over simplyfied led controlls!
void VibratorLegacy::configure(State state, int durationMs) {
    if (state == State::On) {
        m_device.set_sysattr("enable", std::to_string(durationMs));
    } else {
        m_device.set_sysattr("enable", "0");
    }
}
}
