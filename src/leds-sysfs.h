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

#pragma once

#include "leds.h"

#include <map>

#include "udev/udev-cpp.h"

class LedsSysfs : public Leds {

public:
    LedsSysfs();

    static bool usable();
protected:
    void configure() override;

private:
    enum Colors {
        RED = 0,
        GREEN = 1,
        BLUE = 2
    };

    void setDelay();
    void setLed(Colors color, int value);

    std::map<Colors, Udev::UdevDevice> m_rgbDevices;
};
