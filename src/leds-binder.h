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

#include "leds.h"
#include <stdint.h>
#include <gbinder.h>

#define ALIGNED(x) __attribute__((aligned(x)))

enum {
    LIGHT_TYPE_BACKLIGHT = 0,
    LIGHT_TYPE_KEYBOARD = 1,
    LIGHT_TYPE_BUTTONS = 2,
    LIGHT_TYPE_BATTERY = 3,
    LIGHT_TYPE_NOTIFICATIONS = 4,
    LIGHT_TYPE_ATTENTION = 5,
    LIGHT_TYPE_BLUETOOTH = 6,
    LIGHT_TYPE_WIFI = 7,
    LIGHT_TYPE_COUNT = 8,
};

enum {
    FLASH_TYPE_NONE = 0,
    FLASH_TYPE_TIMED = 1,
    FLASH_TYPE_HARDWARE = 2,
};

enum {
    BRIGHTNESS_MODE_USER = 0,
    BRIGHTNESS_MODE_SENSOR = 1,
    BRIGHTNESS_MODE_LOW_PERSISTENCE = 2,
};

struct LightState {
    uint32_t color ALIGNED(4);
    int32_t flashMode ALIGNED(4);
    int32_t flashOnMs ALIGNED(4);
    int32_t flashOffMs ALIGNED(4);
    int32_t brightnessMode ALIGNED(4);
} ALIGNED(4);
static_assert(sizeof(LightState) == 20, "wrong size");

namespace hfd {
class LedsBinder : public Leds {
public:
    LedsBinder();
    ~LedsBinder();

    static bool usable();
protected:
    void configure() override;

private:
    void turnOn();
    void turnOff();
    bool initClient();

    GBinderServiceManager* mSm;  
    GBinderRemoteObject* mRemote;
    GBinderClient* mClient;
};
}
