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
 * File: struct.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "struct.h"

struct AC_struct* AC_struct_make()
{
    struct AC_struct* object = malloc(sizeof(struct AC_struct));
    object->varv = malloc(0);
    object->varc = 0;
    return object;
}

void AC_struct_destroy(struct AC_struct* object)
{
    AC_uint i = 0;
    for(; i < object->varc; i++)
        AC_variable_destroy(object->varv[i]);
    free(object);
}

void AC_struct_addvar(struct AC_struct* object, struct AC_variable* var)
{
    object->varc++;
    object->varv = realloc(object->varv, object->varc * sizeof(void*));
    object->varv[object->varc - 1] = var;
}

void AC_struct_load(struct AC_struct* object, struct AC_scanner* scanner)
{
    AC_scanner_match(scanner, "struct");
    AC_scanner_match(scanner, "{");
    while(AC_token_compare_raw(AC_scanner_get(scanner, 0), "}") == AC_FALSE)
    {
        struct AC_variable* variable = AC_variable_make();
        AC_variable_load(variable, scanner);
        AC_scanner_match(scanner, ";");
        AC_struct_addvar(object, variable);
    }
    AC_scanner_match(scanner, "}");
}

