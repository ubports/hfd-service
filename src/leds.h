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

#include "rgba.h"

#include <memory>

class Leds {
public:
    Leds();

    enum State {
        Off,
        On
    };

    void turnOn(const Rgba &color, int onMs, int offMs);

    void setState(State state);
    State state() const;

    void setColor(const Rgba &color);
    Rgba color() const;

    int onMs() const;
    void setOnMs(int onMs);

    int offMs() const;
    void setOffMs(int offMs); 

    static std::shared_ptr<Leds> create();
    static std::shared_ptr<Leds> create(std::string type);
protected:
    virtual void configure() = 0;

    Rgba m_color;
    State m_state;
    int m_onMs;
    int m_offMs;
    bool m_timer = false;
};
