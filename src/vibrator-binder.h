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
 * Author: Erfan Abdi <erfangplus@gmail.com>
 */

#pragma once

#include "vibrator.h"

#include <memory>
#include <thread>
#include <gbinder.h>

namespace hfd {
class VibratorBinder : public Vibrator {

public:
    VibratorBinder();
    ~VibratorBinder();

    static bool usable();

protected:
    void configure(State state, int durationMs) override;
    
private:
    GBinderServiceManager *mSm;
    GBinderRemoteObject *mRemote;
    GBinderClient *mClient;
    std::shared_ptr<std::thread> m_thread;
};
}
