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

#include <thread>
#include <functional>
#include <unistd.h>
#include <iostream>

namespace hfd {
class RepeatThread {
    
public:
    RepeatThread(std::function<void(void)> func, int sleep, int times)
        : m_func(func)
        , m_times(times)
        , m_sleep(sleep)
        , m_thread(&RepeatThread::runner, this)
    {};

    ~RepeatThread() {
        abort();
    };

    inline void abort() {
        m_break = true;
        m_thread.join();
    };
private:
    inline void runner() {
        for (auto i = 0; i <= m_times; i++) {
            sleep();
            if (m_break)
                break;
            m_func();
        }
    };

    inline void sleep() {
        for (auto i = 0; i <= m_sleep; i++) {
            if (m_break)
                break;
            usleep(1*1000);
        }
    };

    bool m_break = false;
    int m_times;
    int m_sleep;
    std::function<void(void)> m_func;
    std::thread m_thread;
};
}
