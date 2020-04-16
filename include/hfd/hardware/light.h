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

#ifndef HFD_HARDWARE_LIGHT_H_
#define HFD_HARDWARE_LIGHT_H_

#include <hfd/visibility.h>

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <hardware/lights.h>

typedef struct UHardwareLight_* UHardwareLight;

/*
 You must create only one instance per process/application.
*/

HFD_DLL_PUBLIC UHardwareLight
u_hardware_light_new();

HFD_DLL_PUBLIC void
u_hardware_light_setLight(UHardwareLight self, light_state_t state, char const* type);

#ifdef __cplusplus
}
#endif

#endif // HFD_HARDWARE_LIGHT_H_
