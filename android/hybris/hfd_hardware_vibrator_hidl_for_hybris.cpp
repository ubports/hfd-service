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

#include <hfd/hardware/vibrator.h>

#include <pthread.h>
#include <string.h>
#include <unistd.h>

// android stuff
#include <android/hardware/vibrator/1.2/IVibrator.h>

#include <utils/Log.h>
#include <utils/RefBase.h>

using android::OK;
using android::sp;
using android::wp;
using android::status_t;

using android::hardware::Return;
using android::hidl::base::V1_0::IBase;

using android::hardware::vibrator::V1_0::Status;

using IVibrator_V1_0 = android::hardware::vibrator::V1_0::IVibrator;
using IVibrator_V1_1 = android::hardware::vibrator::V1_1::IVibrator;
using IVibrator_V1_2 = android::hardware::vibrator::V1_2::IVibrator;

sp<IVibrator_V1_0> vibratorHal = nullptr;
sp<IVibrator_V1_1> vibratorHal_V1_1 = nullptr;
sp<IVibrator_V1_2> vibratorHal_V1_2 = nullptr;

struct UHardwareVibrator_
{
    bool init();
    void on(uint32_t timeoutMs);
    void off();

    UHardwareVibrator_();
    ~UHardwareVibrator_();
};

UHardwareVibrator_::UHardwareVibrator_()
{
}

UHardwareVibrator_::~UHardwareVibrator_()
{
    if (vibratorHal != nullptr)
        Status status = vibratorHal->off();
}

static void set_vibrator_service_handle() {
    vibratorHal_V1_2 = IVibrator_V1_2::getService();
    if (vibratorHal_V1_2 != nullptr) {
        vibratorHal = vibratorHal_V1_2;
        vibratorHal_V1_1 = vibratorHal_V1_2;
        return;
    }

    ALOGD("vibratorHal 1.2 was null, trying 1.1");
    vibratorHal_V1_1 = IVibrator_V1_1::getService();
    if (vibratorHal_V1_1 != nullptr) {
        vibratorHal = vibratorHal_V1_1;
        return;
    }
    ALOGD("vibratorHal 1.1 was null, trying 1.0");
    vibratorHal = IVibrator_V1_0::getService();
}

bool UHardwareVibrator_::init()
{
    /* Initializes the Vibrator service handle. */
    set_vibrator_service_handle();

    if (vibratorHal == nullptr) {
        ALOGE("Unable to get Vibrator service\n");
        return false;
    }

    return true;
}

void UHardwareVibrator_::on(uint32_t timeoutMs)
{
    if (vibratorHal == nullptr) {
        ALOGE("Unable to get Vibrator service\n");
        return;
    }

    Status status = vibratorHal->on(timeoutMs);
}

void UHardwareVibrator_::off()
{
    if (vibratorHal == nullptr) {
        ALOGE("Unable to get Vibrator service\n");
        return;
    }

    Status status = vibratorHal->off();
}

UHardwareVibrator
u_hardware_vibrator_new()
{
    UHardwareVibrator u_hardware_vibrator = new UHardwareVibrator_();

    // Try ten times to initialize the Vibrator HAL interface,
    // sleeping for 200ms per iteration in case of issues.
    for (unsigned int i = 0; i < 50; i++)
        if (u_hardware_vibrator->init())
            return u_hardware_vibrator;
        else
            // Sleep for some time and leave some time for the system
            // to finish initialization.
            ::usleep(200 * 1000);

    // This is the error case, as we did not succeed in initializing the Vibrator interface.
    return u_hardware_vibrator;
}

void
u_hardware_vibrator_on(UHardwareVibrator self, uint32_t timeoutMs)
{
    self->on(timeoutMs);
}

void
u_hardware_vibrator_off(UHardwareVibrator self)
{
    self->off();
}
