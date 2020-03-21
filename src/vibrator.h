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

#include <memory>

class RepeatThread;
class Vibrator {
public:
    Vibrator();

    enum State {
        Off,
        On
    };

    void vibrate();
    void vibrate(int durationMs);
    void rumble(int durationMs, int repeat);

    int defaultDurationMs() const;
    void setDefaultDurationMs(int durationMs);

    int defaultRepeat() const;
    void setDefaultRepeat(int repeat);


    static std::shared_ptr<Vibrator> create();
    static std::shared_ptr<Vibrator> create(std::string type);
protected:
    virtual void configure(State state, int durationMs) = 0;

    int m_defaultDurationMs;
    int m_defaultRepeat;
    std::shared_ptr<RepeatThread> m_repeatThread;
};
