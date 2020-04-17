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

#include "leds-hybris.h"

#include <iostream>
#include <unistd.h>

extern "C" {
#include <hardware/hardware.h>
#include <hardware/lights.h>
#include <string.h>
}

// legacy hybris support
namespace hfd {

light_device_t* getLightDevice() {
    int err;
    hw_module_t* module;
    light_device_t* lightDevice;

    err = hw_get_module(LIGHTS_HARDWARE_MODULE_ID, (hw_module_t const**)&module);
    if (err == 0) {
        hw_device_t* device;
        err = module->methods->open(module, LIGHT_ID_NOTIFICATIONS, &device);
        if (err == 0 && device) {
            lightDevice = (light_device_t*)device;
            return lightDevice;
        } else {
            std::cout << "Failed to access notification lights" << std::endl;
        }
    } else {
        std::cout << "Failed to initialize lights hardware." << std::endl;
    }

    return nullptr;
}

bool LedsHybris::usable() {
    light_device_t* lightDevice = getLightDevice();
    const bool loaded = (lightDevice != nullptr);

    if (lightDevice) {
        hw_device_t* device = (hw_device_t*) lightDevice;
        device->close(device);
    }

    return loaded;
}

LedsHybris::LedsHybris() : Leds() {
    if (m_lightDevice) {
        return;
    }

    m_lightDevice = getLightDevice();
    if (m_lightDevice)
        turnOff();

    // Get up to date
    configure();
}

LedsHybris::~LedsHybris()
{
    if (m_lightDevice) {
        hw_device_t* device = (hw_device_t*) m_lightDevice;
        device->close(device);
    }
}

void LedsHybris::configure() {
    if (m_state == State::On)
        turnOn();
    else
        turnOff();
}

void LedsHybris::turnOn()
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

    if (!m_lightDevice)
        return;

    if (m_lightDevice->set_light(m_lightDevice, &state) != 0) {
         std::cout << "Failed to turn the light off";
    }
}

void LedsHybris::turnOff()
{
    light_state_t state;
    memset(&state, 0, sizeof(light_state_t));
    state.color = 0x00000000;
    state.flashMode = LIGHT_FLASH_NONE;
    state.flashOnMS = 0;
    state.flashOffMS = 0;
    state.brightnessMode = 0;

    if (!m_lightDevice)
        return;

    if (m_lightDevice->set_light(m_lightDevice, &state) != 0) {
        std::cout << "Failed to turn the light off";
    }
}
}
