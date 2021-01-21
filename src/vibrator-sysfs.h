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

#include "vibrator.h"

#include "udev/udev-cpp.h"

namespace hfd {
class VibratorSysfs : public Vibrator {

public:
    VibratorSysfs();

    static bool usable();
protected:
    void configure(State state, int durationMs) override;
    
private:
    Udev::UdevDevice m_device;
    static bool supportTransient();
};
}
