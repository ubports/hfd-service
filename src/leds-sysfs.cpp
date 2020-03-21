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

#include "leds-sysfs.h"

#include "utils.h"

#include <iostream>
#include <unistd.h>

// if the kernel support led triggers
// we set 

bool LedsSysfs::usable() {
    if (access("/sys/class/leds", F_OK ) == -1)
        return false;

    Udev::Udev udev;
    Udev::UdevEnumerate enumerator = udev.enumerate_new();
    enumerator.add_match_sysattr("subsystem","leds");
    enumerator.scan_devices();

    bool red, green, blue;
    for (auto dev : enumerator.enumerate_devices()) {
        auto splitted = utils::split(dev.get_sysname(), ':');
        if (splitted.size() == 3) {
            auto color = splitted[1];
            std::cout << "got: " << color << std::endl;
            if (color == "red")
                red = true;
            if (color == "green")
                green = true;
            if (color == "blue")
                blue = true;
        }
    }

    return (red && green && blue);
}

LedsSysfs::LedsSysfs(): Leds() {
    Udev::Udev udev;

    Udev::UdevEnumerate enumerator = udev.enumerate_new();
    enumerator.add_match_sysattr("subsystem","leds");
    enumerator.scan_devices();

    for (auto dev : enumerator.enumerate_devices()) {
        auto splitted = utils::split(dev.get_sysname(), ':');
        if (splitted.size() == 3) {
            auto color = splitted[1];
            std::cout << "got: " << color << std::endl;
            if (color == "red")
                m_rgbDevices[Colors::RED] = dev;
            if (color == "green")
                m_rgbDevices[Colors::GREEN] = dev;
            if (color == "blue")
                m_rgbDevices[Colors::BLUE] = dev;
        }
    }

    for (auto dev : m_rgbDevices) {
        dev.second.set_sysattr("trigger", "timer");
    }

    m_timer = true;

    // Get up to date
    configure();
}

// Over simplyfied led controlls!
void LedsSysfs::configure() {
    if (m_state == State::On) {
        std::cout << "red: " << std::to_string(rgba::Red(m_color)) << std::endl;
        std::cout << "green: " << std::to_string(rgba::Green(m_color)) << std::endl;
        std::cout << "blue: " << std::to_string(rgba::Blue(m_color)) << std::endl;

        setDelay();
        setLed(Colors::RED, rgba::Red(m_color));
        setLed(Colors::GREEN, rgba::Green(m_color));
        setLed(Colors::BLUE, rgba::Blue(m_color));
    } else {
        m_rgbDevices[Colors::RED].set_sysattr("brightness", "0");
        m_rgbDevices[Colors::GREEN].set_sysattr("brightness", "0");
        m_rgbDevices[Colors::BLUE].set_sysattr("brightness", "0");
    }
}

void LedsSysfs::setDelay()
{
    if (m_timer) {
        for (auto dev : m_rgbDevices) {
            dev.second.set_sysattr("trigger", "timer");
            dev.second.set_sysattr("delay_on", std::to_string(m_onMs));
            dev.second.set_sysattr("delay_off", std::to_string(m_offMs));
        }
    }
}

void LedsSysfs::setLed(Colors color, int value)
{
    m_rgbDevices[color].set_sysattr("brightness", std::to_string(value));
}
