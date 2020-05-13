/*
 * Copyright (C) 2020 UBports foundation
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

// C APIs
#include <hfd/hardware/light.h>
#include <hfd/hardware/vibrator.h>
#include <hardware/android_hw_module.h>

// Light
IMPLEMENT_FUNCTION0(
UHardwareLight,
u_hardware_light_new);

IMPLEMENT_VOID_FUNCTION3(
u_hardware_light_setLight,
UHardwareLight,
light_state_t,
char const*);

// Vibrator
IMPLEMENT_FUNCTION0(
UHardwareVibrator,
u_hardware_vibrator_new);

IMPLEMENT_VOID_FUNCTION2(
u_hardware_vibrator_on,
UHardwareVibrator,
uint32_t);

IMPLEMENT_VOID_FUNCTION1(
u_hardware_vibrator_off,
UHardwareVibrator);
