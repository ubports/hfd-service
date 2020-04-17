/*
 * Copyright © 2020 UBports foundation
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Erfan Abdi <erfangplus@gmail.com>
 */

#include <hfd/hardware/light.h>

#include <pthread.h>
#include <string.h>
#include <unistd.h>

// android stuff
#include <android/hardware/light/2.0/ILight.h>

#include <utils/Log.h>
#include <utils/RefBase.h>

using android::OK;
using android::sp;
using android::wp;
using android::status_t;

using android::hardware::Return;
using android::hidl::base::V1_0::IBase;
using android::hardware::light::V2_0::LightState;
using android::hardware::light::V2_0::Type;
using android::hardware::light::V2_0::Flash;
using android::hardware::light::V2_0::Brightness;
using android::hardware::light::V2_0::Type;
using android::hardware::light::V2_0::Status;

using ILight_V2_0 = android::hardware::light::V2_0::ILight;

sp<ILight_V2_0> lightHal = nullptr;

struct UHardwareLight_
{
    bool init();
    void setLight(light_state_t state, char const* type);

    UHardwareLight_();
    ~UHardwareLight_();
};

UHardwareLight_::UHardwareLight_()
{
}

UHardwareLight_::~UHardwareLight_()
{
}

static void set_light_service_handle() {
    lightHal = ILight_V2_0::getService();
}

bool UHardwareLight_::init()
{
    /* Initializes the Light service handle. */
    set_light_service_handle();

    if (lightHal == nullptr) {
        ALOGE("Unable to get Light service\n");
        return false;
    }

    return true;
}

void UHardwareLight_::setLight(light_state_t state, char const* type)
{
    if (lightHal == nullptr) {
        ALOGE("Unable to get Light service\n");
        return;
    }
    LightState hidl_state;
    memset(&hidl_state, 0, sizeof(LightState));
    Type hidl_type = Type::NOTIFICATIONS;

    hidl_state.color = state.color;
    switch (state.flashMode) {
        case LIGHT_FLASH_NONE:
            hidl_state.flashMode = Flash::NONE;
            break;
        case LIGHT_FLASH_TIMED:
            hidl_state.flashMode = Flash::TIMED;
            break;
        case LIGHT_FLASH_HARDWARE:
            hidl_state.flashMode = Flash::HARDWARE;
            break;
        default:
            hidl_state.flashMode = Flash::NONE;
            break;
    }
    hidl_state.flashOnMs = state.flashOnMS;
    hidl_state.flashOffMs = state.flashOffMS;
    switch (state.brightnessMode) {
        case BRIGHTNESS_MODE_USER:
            hidl_state.brightnessMode = Brightness::USER;
            break;
        case BRIGHTNESS_MODE_SENSOR:
            hidl_state.brightnessMode = Brightness::SENSOR;
            break;
        case BRIGHTNESS_MODE_LOW_PERSISTENCE:
            hidl_state.brightnessMode = Brightness::LOW_PERSISTENCE;
            break;
        default:
            hidl_state.brightnessMode = Brightness::USER;
            break;
    }
    if (0 == strcmp(LIGHT_ID_BACKLIGHT, type))
        hidl_type = Type::BACKLIGHT;
    else if (0 == strcmp(LIGHT_ID_KEYBOARD, type))
        hidl_type = Type::KEYBOARD;
    else if (0 == strcmp(LIGHT_ID_BUTTONS, type))
        hidl_type = Type::BUTTONS;
    else if (0 == strcmp(LIGHT_ID_BATTERY, type))
        hidl_type = Type::BATTERY;
    else if (0 == strcmp(LIGHT_ID_NOTIFICATIONS, type))
        hidl_type = Type::NOTIFICATIONS;
    else if (0 == strcmp(LIGHT_ID_ATTENTION, type))
        hidl_type = Type::ATTENTION;
    else if (0 == strcmp(LIGHT_ID_BLUETOOTH, type))
        hidl_type = Type::BLUETOOTH;
    else if (0 == strcmp(LIGHT_ID_WIFI, type))
        hidl_type = Type::WIFI;

    ALOGI("setLight: color: %d, flashMode: %d, flashOnMs: %d, flashOffMs: %d, brightnessMode: %d, type: %d\n",
          hidl_state.color, hidl_state.flashMode, hidl_state.flashOnMs, hidl_state.flashOffMs, hidl_state.brightnessMode, hidl_type);

    Status status = lightHal->setLight(hidl_type, hidl_state);
}

UHardwareLight
u_hardware_light_new()
{
    UHardwareLight u_hardware_light = new UHardwareLight_();

    // Try ten times to initialize the Light HAL interface,
    // sleeping for 200ms per iteration in case of issues.
    for (unsigned int i = 0; i < 50; i++)
        if (u_hardware_light->init())
            return u_hardware_light;
        else
            // Sleep for some time and leave some time for the system
            // to finish initialization.
            ::usleep(200 * 1000);

    // This is the error case, as we did not succeed in initializing the Light interface.
    return u_hardware_light;
}

void
u_hardware_light_setLight(UHardwareLight self, light_state_t state, char const* type)
{
    self->setLight(state, type);
}
