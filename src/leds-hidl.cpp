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

#include "leds-hidl.h"

#include <iostream>
#include <unistd.h>

extern "C" {
#include <string.h>
}

namespace hfd {

bool LedsHIDL::usable() {
    return u_hardware_light_new() != NULL;
}

LedsHIDL::LedsHIDL() : Leds() {
    m_lightDevice = u_hardware_light_new();

    // Get up to date
    configure();
}

LedsHIDL::~LedsHIDL()
{
}

void LedsHIDL::configure() {
    if (m_state == State::On)
        turnOn();
    else
        turnOff();
}

void LedsHIDL::turnOn()
{
    // pulse
    light_state_t state;
    memset(&state, 0, sizeof(light_state_t));
    int alpha = (m_color >> 24) & 0xFF;
    int red = (m_color >> 16) & 0xFF;
    int green = (m_color >> 8) & 0xFF;
    int blue = m_color & 0xFF;
    alpha = 0xff; //TODO: Support alpha
    int newcolor = ((alpha & 0xff) << 24) + ((red & 0xff) << 16) + ((green & 0xff) << 8) + (blue & 0xff);
    state.color = newcolor;
    state.flashMode = LIGHT_FLASH_TIMED;
    state.flashOnMS = m_onMs;
    state.flashOffMS = m_offMs;
    state.brightnessMode = BRIGHTNESS_MODE_USER;

    u_hardware_light_setLight(m_lightDevice, state, LIGHT_ID_NOTIFICATIONS);
}

void LedsHIDL::turnOff()
{
    light_state_t state;
    memset(&state, 0, sizeof(light_state_t));
    state.color = 0x00000000;
    state.flashMode = LIGHT_FLASH_NONE;
    state.flashOnMS = 0;
    state.flashOffMS = 0;
    state.brightnessMode = 0;

    u_hardware_light_setLight(m_lightDevice, state, LIGHT_ID_NOTIFICATIONS);
}
}
