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
 * Author: Caleb Connolly <caleb@connolly.tech>
 */

#pragma once

#include <linux/input.h>

#include "vibrator.h"
#include "udev/udev-cpp.h"

// Borrowed from linuxconsole/utils/bitmaskros.h
/* Number of bits for 1 unsigned char */
#define nBitsPerUchar          (sizeof(unsigned char) * 8)
/* Index=Offset of given bit in 1 unsigned char */
#define bitOffsetInUchar(bit)  ((bit)%nBitsPerUchar)
/* Index=Offset of the unsigned char associated to the bit
 * at the given index=offset
 */
#define ucharIndexForBit(bit)  ((bit)/nBitsPerUchar)
/* Test the bit with given index=offset in an unsigned char array */
#define testBit(bit, array)    ((array[ucharIndexForBit(bit)] >> bitOffsetInUchar(bit)) & 1)

namespace hfd {
class VibratorFF : public Vibrator {

public:
    VibratorFF();
    ~VibratorFF();

    static bool usable();
    static std::string getFirstFFDevice();
protected:
    void configure(State state, int durationMs) override;
    
private:
    struct ff_effect effect;
    std::string devname;
    int fd;
};
}
