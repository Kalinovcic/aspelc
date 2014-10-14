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
 * File: identifier.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "identifier.h"

struct AC_identifier* AC_identifier_make()
{
    struct AC_identifier* object = malloc(sizeof(struct AC_identifier));
    object->sub = AC_NULL;
    return object;
}

void AC_identifier_destroy(struct AC_identifier* object)
{
    if(AC_identifier_hassub(object) == AC_TRUE)
        AC_identifier_destroy(object->sub);
    free(object);
}

AC_bool AC_identifier_hassub(struct AC_identifier* object)
{
    return object->sub != AC_NULL;
}

void AC_identifier_setsub(struct AC_identifier* object, struct AC_identifier* sub)
{
    assert(AC_identifier_hassub(object) == AC_FALSE);
    object->sub = sub;
}
struct AC_identifier* AC_identifier_getsub(struct AC_identifier* object)
{
    return object->sub;
}

void AC_identifier_load(struct AC_identifier* object, struct AC_scanner* scanner)
{
    struct AC_token name = AC_scanner_getword(scanner, 0);
    AC_scanner_next(scanner);
    object->name = name;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), ":") == AC_TRUE)
    {
        AC_scanner_match(scanner, ":");
        struct AC_identifier* sub = AC_identifier_make();
        AC_identifier_load(sub, scanner);
        object->sub = sub;
    }
}

void AC_identifier_print(struct AC_identifier* object)
{
    printf("%.*s", object->name.tokenl, object->name.token);
    if(AC_identifier_hassub(object))
    {
        printf("::");
        AC_identifier_print(object->sub);
    }
}
