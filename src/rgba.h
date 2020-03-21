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

typedef unsigned int Rgba;  

namespace rgba {
    inline int Red(Rgba rgb) { return ((rgb >> 16) & 0xff); }
    inline int Green(Rgba rgb) { return ((rgb >> 8) & 0xff); }
    inline int Blue(Rgba rgb) { return (rgb & 0xff); }
    inline int Alpha(Rgba rgb) { return rgb >> 24; }
}
