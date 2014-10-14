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
 * File: struct.h
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef STRUCT_H_
#define STRUCT_H_

#include <stdlib.h>

#include "acdef.h"
#include "program.h"
#include "scanner.h"
#include "type.h"
#include "var.h"

struct AC_struct
{
    struct AC_variable** varv;
    AC_uint varc;
};

#include "program.h"

struct AC_struct* AC_struct_make();
void AC_struct_destroy(struct AC_struct* object);

void AC_struct_addvar(struct AC_struct* object, struct AC_variable* var);

void AC_struct_load(struct AC_struct* object, struct AC_scanner* scanner);


#endif /* STRUCT_H_ */
