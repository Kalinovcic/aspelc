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

void AC_primitive_print(enum AC_primitive object)
{
    switch(object)
    {
    case AC_PRIMITIVE_BOOL: fprintf(stderr, "bool"); break;
    case AC_PRIMITIVE_BYTE: fprintf(stderr, "byte"); break;
    case AC_PRIMITIVE_UBYTE: fprintf(stderr, "ubyte"); break;
    case AC_PRIMITIVE_SHORT: fprintf(stderr, "short"); break;
    case AC_PRIMITIVE_USHORT: fprintf(stderr, "ushort"); break;
    case AC_PRIMITIVE_INT: fprintf(stderr, "int"); break;
    case AC_PRIMITIVE_UINT: fprintf(stderr, "uint"); break;
    case AC_PRIMITIVE_FLOAT: fprintf(stderr, "float"); break;
    case AC_PRIMITIVE_LONG: fprintf(stderr, "long"); break;
    case AC_PRIMITIVE_ULONG: fprintf(stderr, "ulong"); break;
    case AC_PRIMITIVE_DOUBLE: fprintf(stderr, "double"); break;
    }
}

AC_bool AC_primitive_integer(enum AC_primitive object)
{
    if(object == AC_PRIMITIVE_BYTE ||
       object == AC_PRIMITIVE_UBYTE ||
       object == AC_PRIMITIVE_SHORT ||
       object == AC_PRIMITIVE_USHORT ||
       object == AC_PRIMITIVE_INT ||
       object == AC_PRIMITIVE_UINT ||
       object == AC_PRIMITIVE_LONG ||
       object == AC_PRIMITIVE_ULONG) return AC_TRUE;
    return AC_FALSE;
}

AC_bool AC_primitive_signed(enum AC_primitive object)
{
    if(object == AC_PRIMITIVE_BYTE ||
       object == AC_PRIMITIVE_SHORT ||
       object == AC_PRIMITIVE_INT ||
       object == AC_PRIMITIVE_LONG) return AC_TRUE;
    return AC_FALSE;
}

AC_bool AC_primitive_unsigned(enum AC_primitive object)
{
    if(object == AC_PRIMITIVE_UBYTE ||
       object == AC_PRIMITIVE_USHORT ||
       object == AC_PRIMITIVE_UINT ||
       object == AC_PRIMITIVE_ULONG) return AC_TRUE;
    return AC_FALSE;
}

AC_ulong AC_primitive_size(enum AC_primitive object)
{
    switch(object)
    {
    case AC_PRIMITIVE_BOOL:
    case AC_PRIMITIVE_BYTE: case AC_PRIMITIVE_UBYTE:
    case AC_PRIMITIVE_SHORT: case AC_PRIMITIVE_USHORT:
    case AC_PRIMITIVE_INT: case AC_PRIMITIVE_UINT:
    case AC_PRIMITIVE_FLOAT:
        return 4;
    case AC_PRIMITIVE_LONG: case AC_PRIMITIVE_ULONG:
    case AC_PRIMITIVE_DOUBLE:
        return 8;
    }
    return 0;
}

AC_uint AC_primitive_prec(enum AC_primitive object)
{
    switch(object)
    {
    case AC_PRIMITIVE_BOOL: return 0;
    case AC_PRIMITIVE_BYTE: return 1;
    case AC_PRIMITIVE_UBYTE: return 1;
    case AC_PRIMITIVE_SHORT: return 2;
    case AC_PRIMITIVE_USHORT: return 2;
    case AC_PRIMITIVE_INT: return 3;
    case AC_PRIMITIVE_UINT: return 3;
    case AC_PRIMITIVE_LONG: return 4;
    case AC_PRIMITIVE_ULONG: return 4;
    case AC_PRIMITIVE_FLOAT: return 5;
    case AC_PRIMITIVE_DOUBLE: return 6;
    }
    return 255;
}

enum AC_primitive AC_primitive_greater(enum AC_primitive p1, enum AC_primitive p2)
{
    AC_uint p1prec = AC_primitive_prec(p1);
    AC_uint p2prec = AC_primitive_prec(p2);
    if(p1prec > p2prec) return p1;
    return p2;
}

struct AC_typename* AC_typename_stackconv(struct AC_typename* top, struct AC_typename* bottom, AC_uint srcline, struct AC_output* output)
{
    if(top->type != AC_TYPENAME_PRIMITIVE || bottom->type != AC_TYPENAME_PRIMITIVE)
        AC_invalid_conversion(top, bottom, srcline);

    enum AC_primitive rawtop = top->value.primitive;
    enum AC_primitive rawbottom = bottom->value.primitive;

    if(rawtop == rawbottom)
    {
        AC_typename_destroy(bottom);
        return top;
    }

    if(rawtop == AC_PRIMITIVE_BOOL || rawbottom == AC_PRIMITIVE_BOOL)
        AC_invalid_conversion(top, bottom, srcline);

    enum AC_primitive ptop = rawtop;
    if(ptop == AC_PRIMITIVE_BYTE || ptop == AC_PRIMITIVE_SHORT)
        ptop = AC_PRIMITIVE_INT;
    else if(ptop == AC_PRIMITIVE_UBYTE || ptop == AC_PRIMITIVE_USHORT)
        ptop = AC_PRIMITIVE_UINT;

    enum AC_primitive pbottom = rawbottom;
    if(pbottom == AC_PRIMITIVE_BYTE || pbottom == AC_PRIMITIVE_SHORT)
        pbottom = AC_PRIMITIVE_INT;
    else if(pbottom == AC_PRIMITIVE_UBYTE || pbottom == AC_PRIMITIVE_USHORT)
        pbottom = AC_PRIMITIVE_UINT;

    if(ptop == pbottom)
    {
        enum AC_primitive greater = AC_primitive_greater(rawtop, rawbottom);
        if(greater == rawtop)
        {
            AC_typename_destroy(bottom);
            return top;
        }
        AC_typename_destroy(top);
        return bottom;
    }

    if((AC_primitive_signed(ptop) == AC_TRUE && AC_primitive_unsigned(pbottom) == AC_TRUE)
    || (AC_primitive_unsigned(ptop) == AC_TRUE && AC_primitive_signed(pbottom) == AC_TRUE))
    {
        AC_report("integer signedness error near line %d\n", srcline);
        AC_report_abort();
    }

    if(ptop == AC_PRIMITIVE_UINT)
        ptop = AC_PRIMITIVE_INT;
    else if(ptop == AC_PRIMITIVE_ULONG)
        ptop = AC_PRIMITIVE_LONG;

    if(pbottom == AC_PRIMITIVE_UINT)
        pbottom = AC_PRIMITIVE_INT;
    else if(pbottom == AC_PRIMITIVE_ULONG)
        pbottom = AC_PRIMITIVE_LONG;

    enum AC_primitive greater = AC_primitive_greater(ptop, pbottom);
    AC_bool swap = (ptop == greater) ? AC_TRUE : AC_FALSE;

    if(swap == AC_TRUE)
    {
        AC_codegen_swap(AC_primitive_size(ptop), AC_primitive_size(pbottom), output);

        enum AC_primitive buff = ptop;
        ptop = pbottom;
        pbottom = buff;
    }

    if(AC_primitive_integer(ptop) == AC_TRUE && AC_primitive_integer(pbottom) == AC_TRUE)
        AC_output_write(output, "ci%d%d", AC_primitive_size(ptop), AC_primitive_size(pbottom));
    else if(AC_primitive_integer(ptop) == AC_TRUE)
    {
        if(AC_primitive_size(ptop) != AC_primitive_size(pbottom))
            AC_output_write(output, "ci%d%d", AC_primitive_size(ptop), AC_primitive_size(pbottom));
        AC_output_write(output, "cif%d", AC_primitive_size(pbottom));
    }
    else AC_output_write(output, "cf%d%d", AC_primitive_size(ptop), AC_primitive_size(pbottom));

    ptop = pbottom;

    if(swap == AC_TRUE)
    {
        AC_codegen_swap(AC_primitive_size(ptop), AC_primitive_size(pbottom), output);
        AC_typename_destroy(bottom);
        return top;
    }

    AC_typename_destroy(top);
    return bottom;
}

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
    case AC_TYPENAME_POINTER:
        if(object->value.pointer != AC_NULL)
            AC_typename_destroy(object->value.pointer);
        break;
    }
    free(object);
}

void AC_typename_destroy_noptr(struct AC_typename* object)
{
    switch(object->type)
    {
    case AC_TYPENAME_PRIMITIVE: break;
    case AC_TYPENAME_USERTYPE: AC_identifier_destroy(object->value.usertype); break;
    case AC_TYPENAME_POINTER: break;
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

void AC_typename_print(struct AC_typename* object)
{
    switch(object->type)
    {
    case AC_TYPENAME_PRIMITIVE:
        AC_primitive_print(object->value.primitive);
        break;
    case AC_TYPENAME_USERTYPE:
        AC_identifier_print(object->value.usertype);
        break;
    case AC_TYPENAME_POINTER:
        AC_typename_print(object->value.pointer);
        printf("*");
        break;
    }
}

AC_ulong AC_typename_size(struct AC_typename* object, struct AC_program* program)
{
    switch(object->type)
    {
    case AC_TYPENAME_PRIMITIVE:
        return AC_primitive_size(object->value.primitive);
    case AC_TYPENAME_POINTER:
        return 8;
    case AC_TYPENAME_USERTYPE:
        return AC_complex_size(AC_program_findcomplex(program, object->value.usertype), program);
    }
    assert(0);
    return 0;
}

AC_bool AC_typename_isbool(struct AC_typename* typename)
{
    if(typename->type == AC_TYPENAME_PRIMITIVE)
    {
        if(typename->value.primitive == AC_PRIMITIVE_BOOL)
            return AC_TRUE;
        else
            return AC_FALSE;
    }
    return AC_FALSE;
}

AC_bool AC_typename_isinteger(struct AC_typename* typename)
{
    if(typename->type == AC_TYPENAME_PRIMITIVE)
        return AC_primitive_integer(typename->value.primitive);
    return AC_FALSE;
}

AC_bool AC_typename_issigned(struct AC_typename* typename)
{
    if(typename->type == AC_TYPENAME_PRIMITIVE)
        return AC_primitive_signed(typename->value.primitive);
    return AC_FALSE;
}

AC_bool AC_typename_isunsigned(struct AC_typename* typename)
{
    if(typename->type == AC_TYPENAME_PRIMITIVE)
        return AC_primitive_unsigned(typename->value.primitive);
    return AC_FALSE;
}

AC_bool AC_typename_isfloat(struct AC_typename* typename)
{
    if(typename->type == AC_TYPENAME_PRIMITIVE)
    {
        if(typename->value.primitive == AC_PRIMITIVE_FLOAT
        || typename->value.primitive == AC_PRIMITIVE_DOUBLE)
            return AC_TRUE;
        else
            return AC_FALSE;
    }
    return AC_FALSE;
}

AC_bool AC_typename_isnumber(struct AC_typename* typename)
{
    return AC_typename_isinteger(typename) || AC_typename_isfloat(typename);
}

AC_bool AC_typename_ispointer(struct AC_typename* typename)
{
    if(typename->type == AC_TYPENAME_POINTER)
        return AC_TRUE;
    return AC_FALSE;
}

struct AC_complex* AC_complex_make()
{
    struct AC_complex* object = malloc(sizeof(struct AC_complex));
    return object;
}

void AC_complex_destroy(struct AC_complex* object)
{
    switch(object->type)
    {
    case AC_COMPLEX_STRUCT: AC_struct_destroy(object->value.zstruct); break;
    case AC_COMPLEX_DELEGATE: break;
    }
    free(object);
}

void AC_complex_load(struct AC_complex* object, struct AC_scanner* scanner)
{
    struct AC_token token = AC_scanner_getword(scanner, 0);

    if(AC_token_compare_raw(token, "struct") == AC_TRUE)
    {
        object->type = AC_COMPLEX_STRUCT;
        object->value.zstruct = AC_struct_make();
        AC_struct_load(object->value.zstruct, scanner);
    }
    else assert(0);

    object->name = AC_scanner_getword(scanner, 0);
    AC_scanner_next(scanner);

    AC_scanner_match(scanner, ";");
}

AC_ulong AC_complex_size(struct AC_complex* object, struct AC_program* program)
{
    switch(object->type)
    {
    case AC_COMPLEX_STRUCT: return AC_struct_size(object->value.zstruct, program);
    case AC_COMPLEX_DELEGATE: return 8; // TODO: ?
    }
    assert(0);
    return 0;
}
