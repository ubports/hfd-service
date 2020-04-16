/*
 * Copyright (C) 2020 UBports foundation
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
 * Authored by: Erfan Abdi <erfangplus@gmail.com>
 *
 */

#ifndef HFD_VISIBILITY_H_
#define HFD_VISIBILITY_H_

#define HFD_DLL_PUBLIC __attribute__ ((visibility ("default")))
#define HFD_DLL_LOCAL  __attribute__ ((visibility ("hidden")))

#endif // HFD_VISIBILITY_H_
