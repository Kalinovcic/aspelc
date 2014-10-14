/* 
 * Copyright (C) 2014 Lovro Kalinovcic
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 * File: acdef.h
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef ACDEF_H_
#define ACDEF_H_

#include <stdint.h>

typedef int8_t AC_byte;
typedef int16_t AC_short;
typedef int32_t AC_int;
typedef int64_t AC_long;

typedef uint8_t AC_ubyte;
typedef uint16_t AC_ushort;
typedef uint32_t AC_uint;
typedef uint64_t AC_ulong;

typedef uint8_t AC_bool;

#define AC_NULL 0

#define AC_TRUE 1
#define AC_FALSE 0

typedef float AC_float;
typedef double AC_double;

typedef void* (*AC_make)(void);

#endif /* ACDEF_H_ */
