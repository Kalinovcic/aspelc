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
 * File: var.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "var.h"

struct AC_variable* AC_variable_make()
{
    struct AC_variable* object = malloc(sizeof(struct AC_variable));
    object->typename = AC_typename_make();
    return object;
}

void AC_variable_destroy(struct AC_variable* object)
{
    AC_typename_destroy(object->typename);
    free(object);
}

void AC_variable_load(struct AC_variable* object, struct AC_scanner* scanner)
{
    AC_typename_load(object->typename, scanner);
    object->name = AC_scanner_getword(scanner, 0);
    AC_scanner_next(scanner);
}

