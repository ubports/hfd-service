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

#include "vibrator.h"
#include "vibrator-ff.h"
#include "vibrator-sysfs.h"
#include "vibrator-legacy.h"
#include "repeatThread.h"

#include <iostream>

namespace hfd {
class VibratorDummy : public Vibrator {
public:
    VibratorDummy() = default;
protected:
    void configure(State state, int durationMs) override {
        if (state == State::On) {
            std::cout << "VibratorDummy on, duration: " << durationMs << std::endl;
        } else {
            std::cout << "VibratorDummy off" << std::endl;
        }
    };
};


std::shared_ptr<Vibrator> Vibrator::create()
{
    if (VibratorSysfs::usable()) {
        std::cout << "Using sysfs vibrator" << std::endl;
        return std::make_shared<VibratorSysfs>();
    }
    else if (VibratorLegacy::usable()) {
        std::cout << "Using legacy vibrator" << std::endl;
        return std::make_shared<VibratorLegacy>();
    }
    else if (VibratorFF::usable()) {
        std::cout << "Using force feedback vibrator" << std::endl;
        return std::make_shared<VibratorFF>();
    }

    std::cout << "Using dummy vibrator" << std::endl;
    return std::make_shared<VibratorDummy>();
}

std::shared_ptr<Vibrator> Vibrator::create(std::string type)
{
    if (type == "ff") {
        std::cout << "Using force feedback vibrator" << std::endl;
        return std::make_shared<VibratorFF>();  
    } else if (type == "sysfs") {
        std::cout << "Using sysfs vibrator" << std::endl;
        return std::make_shared<VibratorSysfs>();  
    } else if (type == "legacy") {
        std::cout << "Using legacy vibrator" << std::endl;
        return std::make_shared<VibratorLegacy>();
    }

    std::cout << "Using dummy vibrator" << std::endl;
    return std::make_shared<VibratorDummy>();
}


Vibrator::Vibrator()
  : m_defaultDurationMs(500),
    m_defaultRepeat(1)
{
}

void Vibrator::vibrate()
{
    vibrate(m_defaultDurationMs);
}

void Vibrator::vibrate(unsigned int durationMs)
{
    configure(State::On, durationMs);
}

void Vibrator::vibratePattern(unsigned int durationMs[], unsigned int repeat)
{
    std::cout << "vibratePattern() called";
}

void Vibrator::rumble(unsigned int durationMs, unsigned int repeat)
{
    m_repeatThread = std::make_shared<RepeatThread>([=](){
        vibrate(durationMs);
    }, durationMs*2, repeat);
}

int Vibrator::defaultDurationMs() const
{
    return m_defaultDurationMs;
}

void Vibrator::setDefaultDurationMs(int durationMs)
{
    if (m_defaultDurationMs != durationMs) {
        m_defaultDurationMs = durationMs;
    }
}

int Vibrator::defaultRepeat() const
{
    return m_defaultRepeat;
}

void Vibrator::setDefaultRepeat(int repeat)
{
    if (m_defaultRepeat != repeat) {
        m_defaultRepeat = repeat;
    }
}
}
