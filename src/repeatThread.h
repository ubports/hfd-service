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
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>

namespace hfd {
class RepeatThread {
    
public:
    RepeatThread(std::function<void(void)> func, int sleep, int times)
        : m_func(func)
        , m_times(times)
        , m_sleepMs(sleep)
        , m_thread(&RepeatThread::runner, this)
    {};

    ~RepeatThread() {
        abort();
    };

    inline void abort() {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_break = true;
            m_cv.notify_all();
        }
        m_thread.join();
    };
private:
    inline void runner() {
        for (auto i = 0; i <= m_times; i++) {
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                auto timeout = std::chrono::steady_clock::now() + m_sleepMs;

                bool is_stopped = m_cv.wait_until(lock, timeout,
                                    [&]() { return m_break; });
                if (is_stopped)
                    break;
            }

            // Call m_func() without the lock; we don't support canceling
            // in the middle.
            m_func();
        }
    };

    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_break = false;

    int m_times;
    std::chrono::milliseconds m_sleepMs;
    std::function<void(void)> m_func;
    std::thread m_thread;
};
}
