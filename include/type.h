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
 * File: type.h
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef TYPE_H_
#define TYPE_H_

#include <assert.h>
#include <stdlib.h>

#include "identifier.h"
#include "token.h"
#include "scanner.h"
#include "struct.h"

enum AC_primitive
{
    AC_PRIMITIVE_BYTE, AC_PRIMITIVE_UBYTE,
    AC_PRIMITIVE_SHORT, AC_PRIMITIVE_USHORT,
    AC_PRIMITIVE_INT, AC_PRIMITIVE_UINT, AC_PRIMITIVE_FLOAT,
    AC_PRIMITIVE_LONG, AC_PRIMITIVE_ULONG, AC_PRIMITIVE_DOUBLE,
};

enum AC_typename_type
{
    AC_TYPENAME_PRIMITIVE,
    AC_TYPENAME_USERTYPE,
    AC_TYPENAME_POINTER,
};

union AC_typename_union
{
    enum AC_primitive primitive;
    struct AC_identifier* usertype;
    struct AC_typename* pointer;
};

struct AC_typename
{
    enum AC_typename_type type;
    union AC_typename_union value;
};

struct AC_typename* AC_typename_make();
void AC_typename_destroy(struct AC_typename* object);

void AC_typename_load(struct AC_typename* object, struct AC_scanner* scanner);

enum AC_basetype_type
{
    AC_BASETYPE_EXISTING,
    AC_BASETYPE_CREATED,
};

union AC_basetype_union
{
    struct AC_typename* existing;
    struct AC_struct* created;
};

struct AC_basetype
{
    enum AC_basetype_type type;
    union AC_basetype_union value;
};

struct AC_basetype* AC_basetype_make();
void AC_basetype_destroy(struct AC_basetype* object);

void AC_basetype_load(struct AC_basetype* object, struct AC_scanner* scanner);

struct AC_type
{
    struct AC_token name;
    struct AC_basetype* basetype;
};

struct AC_type* AC_type_make();
void AC_type_destroy(struct AC_type* object);

void AC_type_load(struct AC_type* object, struct AC_scanner* scanner);

#endif /* TYPE_H_ */
