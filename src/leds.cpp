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

#include "leds.h"
#include "leds-sysfs.h"

#ifdef HAVE_LIBHYBRIS
#include "leds-hybris.h"
#endif
#ifdef HAVE_LIBGBINDER
#include "leds-binder.h"
#endif

#include <iostream>

namespace hfd {
class LedsDummy : public Leds {
public:
    LedsDummy() = default;
protected:
    void configure() override {
        if (m_state == State::On) {
            std::cout << "LedsDummy on, color: " << m_color << std::endl;
        } else {
            std::cout << "LedsDummy off" << std::endl;
        }
    };
};

std::shared_ptr<Leds> Leds::create()
{
#ifdef HAVE_LIBGBINDER
    if (LedsBinder::usable()) {
        std::cout << "Using binder leds" << std::endl;
        return std::make_shared<LedsBinder>();
    }
    else
#endif
#ifdef HAVE_LIBHYBRIS
    if (LedsHybris::usable()) {
        std::cout << "Using hybris leds" << std::endl;
        return std::make_shared<LedsHybris>();
    }
    else
#endif
    if (LedsSysfs::usable()) {
        std::cout << "Using sysfs leds" << std::endl;
        return std::make_shared<LedsSysfs>();
    }

    std::cout << "Using dummy leds" << std::endl;
    return std::make_shared<LedsDummy>();
}

std::shared_ptr<Leds> Leds::create(std::string type)
{
    if (type == "sysfs") {
        std::cout << "Using sysfs leds" << std::endl;
        return std::make_shared<LedsSysfs>();
    }
#ifdef HAVE_LIBHYBRIS
    else if (type == "hybris") {
        std::cout << "Using hybris leds" << std::endl;
        return std::make_shared<LedsHybris>();
    }
#endif
#ifdef HAVE_LIBGBINDER
    else if (type == "binder") {
        std::cout << "Using binder leds" << std::endl;
        return std::make_shared<LedsBinder>();
    }
#endif

    std::cout << "Using dummy leds" << std::endl;
    return std::make_shared<LedsDummy>();
}

Leds::Leds()
  : m_color(0x00ff0080),
    m_state(State::Off),
    m_onMs(1000),
    m_offMs(3000)
{
}

void Leds::setState(State newState)
{
    m_state = newState;
    configure();
}

State Leds::state() const
{
    return m_state;
}

void Leds::setColor(const Rgba &color)
{
    if (m_color != color) {
        m_color = color;
        if (m_state == State::On)
            configure();
    }
}

Rgba Leds::color() const
{
    return m_color;
}

int Leds::onMs() const
{
    return m_onMs;
}

void Leds::setOnMs(int onMs)
{
    if (m_onMs != onMs) {
        m_onMs = onMs;
        if (m_state == State::On)
            configure();
    }
}

int Leds::offMs() const
{
    return m_offMs;
}

void Leds::setOffMs(int offMs)
{
    if (m_offMs != offMs) {
        m_offMs = offMs;
        if (m_state == State::On)
            configure();
    }
}
}
