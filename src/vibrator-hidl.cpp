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
 * Author: Erfan Abdi <erfangplus@gmail.com>
 */

#include "vibrator-hidl.h"

#include <iostream>
#include <unistd.h>

namespace hfd {

bool VibratorHIDL::usable() {
    return u_hardware_vibrator_new() != NULL;
}

VibratorHIDL::VibratorHIDL(): Vibrator() {
    m_device = u_hardware_vibrator_new();

    // Make sure we are off on init
    configure(State::Off, 0);
}

VibratorHIDL::~VibratorHIDL()
{
    if (m_thread) {
        m_thread->join();
    }
}

void VibratorHIDL::configure(State state, int durationMs) {
    if (state == State::On) {
        u_hardware_vibrator_on(m_device, durationMs);
    } else {
        u_hardware_vibrator_off(m_device);
    }
}
}
