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
 * File: type.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "type.h"

struct AC_typename* AC_typename_make()
{
    struct AC_typename* object = malloc(sizeof(struct AC_typename));
    return object;
}
void AC_typename_destroy(struct AC_typename* object)
{
    switch(object->type)
    {
    case AC_TYPENAME_PRIMITIVE: break;
    case AC_TYPENAME_USERTYPE: AC_identifier_destroy(object->value.usertype); break;
    case AC_TYPENAME_POINTER: AC_typename_destroy(object->value.pointer); break;
    }
    free(object);
}

static AC_bool AC_typename_primitivetoken(struct AC_token token)
{
    if(AC_token_compare_raw(token, "byte")) return AC_TRUE;
    if(AC_token_compare_raw(token, "ubyte")) return AC_TRUE;
    if(AC_token_compare_raw(token, "short")) return AC_TRUE;
    if(AC_token_compare_raw(token, "ushort")) return AC_TRUE;
    if(AC_token_compare_raw(token, "int")) return AC_TRUE;
    if(AC_token_compare_raw(token, "uint")) return AC_TRUE;
    if(AC_token_compare_raw(token, "float")) return AC_TRUE;
    if(AC_token_compare_raw(token, "long")) return AC_TRUE;
    if(AC_token_compare_raw(token, "ulong")) return AC_TRUE;
    if(AC_token_compare_raw(token, "double")) return AC_TRUE;
    return AC_FALSE;
}

static enum AC_primitive AC_typename_toprimitive(struct AC_token token)
{
    if(AC_token_compare_raw(token, "byte")) return AC_PRIMITIVE_BYTE;
    if(AC_token_compare_raw(token, "ubyte")) return AC_PRIMITIVE_UBYTE;
    if(AC_token_compare_raw(token, "short")) return AC_PRIMITIVE_SHORT;
    if(AC_token_compare_raw(token, "ushort")) return AC_PRIMITIVE_USHORT;
    if(AC_token_compare_raw(token, "int")) return AC_PRIMITIVE_INT;
    if(AC_token_compare_raw(token, "uint")) return AC_PRIMITIVE_UINT;
    if(AC_token_compare_raw(token, "float")) return AC_PRIMITIVE_FLOAT;
    if(AC_token_compare_raw(token, "long")) return AC_PRIMITIVE_LONG;
    if(AC_token_compare_raw(token, "ulong")) return AC_PRIMITIVE_ULONG;
    if(AC_token_compare_raw(token, "double")) return AC_PRIMITIVE_DOUBLE;
    assert(0);
    return AC_PRIMITIVE_INT;
}

void AC_typename_load(struct AC_typename* object, struct AC_scanner* scanner)
{
    struct AC_token token = AC_scanner_getword(scanner, 0);

    if(AC_typename_primitivetoken(token) == AC_TRUE)
    {
        object->type = AC_TYPENAME_PRIMITIVE;
        object->value.primitive = AC_typename_toprimitive(token);
        AC_scanner_next(scanner);
    }
    else
    {
        object->type = AC_TYPENAME_USERTYPE;
        object->value.usertype = AC_identifier_make();
        AC_identifier_load(object->value.usertype, scanner);
    }

    do
    {
        struct AC_token token = AC_scanner_get(scanner, 0);
        if((*(token.token)) != '*') break;
        AC_uint i = 0;
        for(; i < token.tokenl; i++)
        {
            assert(*(token.token + i) == '*');
            struct AC_typename* typename = AC_typename_make();
            typename->type = object->type;
            typename->value = object->value;

            object->type = AC_TYPENAME_POINTER;
            object->value.pointer = typename;
        }
        AC_scanner_next(scanner);
    } while(1);
}

struct AC_basetype* AC_basetype_make()
{
    struct AC_basetype* object = malloc(sizeof(struct AC_basetype));
    return object;
}
void AC_basetype_destroy(struct AC_basetype* object)
{
    switch(object->type)
    {
    case AC_BASETYPE_EXISTING: AC_typename_destroy(object->value.existing); break;
    case AC_BASETYPE_CREATED: AC_struct_destroy(object->value.created); break;
    }
    free(object);
}

void AC_basetype_load(struct AC_basetype* object, struct AC_scanner* scanner)
{
    struct AC_token token = AC_scanner_getword(scanner, 0);

    if(AC_token_compare_raw(token, "struct"))
    {
        object->type = AC_BASETYPE_CREATED;
        object->value.created = AC_struct_make();
        AC_struct_load(object->value.created, scanner);
    }
    else
    {
        object->type = AC_BASETYPE_EXISTING;
        object->value.existing = AC_typename_make();
        AC_typename_load(object->value.existing, scanner);
    }
}

struct AC_type* AC_type_make()
{
    struct AC_type* object = malloc(sizeof(struct AC_type));
    object->basetype = AC_basetype_make();
    return object;
}
void AC_type_destroy(struct AC_type* object)
{
    AC_basetype_destroy(object->basetype);
    free(object);
}

void AC_type_load(struct AC_type* object, struct AC_scanner* scanner)
{
    AC_scanner_match(scanner, "type");
    AC_basetype_load(object->basetype, scanner);
    object->name = AC_scanner_getword(scanner, 0);
    AC_scanner_next(scanner);
    AC_scanner_match(scanner, ";");
}
