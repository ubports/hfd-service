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

#ifndef HFD_HARDWARE_VIBRATOR_H_
#define HFD_HARDWARE_VIBRATOR_H_

#include <hfd/visibility.h>

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UHardwareVibrator_* UHardwareVibrator;

/*
 You must create only one instance per process/application.
*/

HFD_DLL_PUBLIC UHardwareVibrator
u_hardware_vibrator_new();

HFD_DLL_PUBLIC void
u_hardware_vibrator_on(UHardwareVibrator self, uint32_t timeoutMs);

HFD_DLL_PUBLIC void
u_hardware_vibrator_off(UHardwareVibrator self);

#ifdef __cplusplus
}
#endif

#endif // HFD_HARDWARE_VIBRATOR_H_
