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

enum AC_primitive
{
    AC_PRIMITIVE_BOOL, AC_PRIMITIVE_BYTE, AC_PRIMITIVE_UBYTE,
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

#include <assert.h>
#include <stdlib.h>

#include "codegen.h"
#include "identifier.h"
#include "output.h"
#include "token.h"
#include "scanner.h"
#include "struct.h"

void AC_primitive_print(enum AC_primitive object);
AC_bool AC_primitive_integer(enum AC_primitive object);
AC_bool AC_primitive_signed(enum AC_primitive object);
AC_bool AC_primitive_unsigned(enum AC_primitive object);
AC_ulong AC_primitive_size(enum AC_primitive object);
AC_uint AC_primitive_prec(enum AC_primitive object);
enum AC_primitive AC_primitive_greater(enum AC_primitive p1, enum AC_primitive p2);
struct AC_typename* AC_typename_stackconv(struct AC_typename* top, struct AC_typename* bottom, AC_uint srcline, struct AC_output* output);

struct AC_typename* AC_typename_make();
void AC_typename_destroy(struct AC_typename* object);
void AC_typename_destroy_noptr(struct AC_typename* object);

void AC_typename_load(struct AC_typename* object, struct AC_scanner* scanner);
void AC_typename_print(struct AC_typename* object);

AC_ulong AC_typename_size(struct AC_typename* object, struct AC_program* program);

AC_bool AC_typename_isbool(struct AC_typename* typename);
AC_bool AC_typename_isinteger(struct AC_typename* typename);
AC_bool AC_typename_issigned(struct AC_typename* typename);
AC_bool AC_typename_isunsigned(struct AC_typename* typename);
AC_bool AC_typename_isfloat(struct AC_typename* typename);
AC_bool AC_typename_isnumber(struct AC_typename* typename);
AC_bool AC_typename_ispointer(struct AC_typename* typename);

enum AC_complex_type
{
    AC_COMPLEX_STRUCT,
    AC_COMPLEX_DELEGATE,
};

union AC_complex_union
{
    struct AC_struct* zstruct;
    void* delegate;     // TODO: delegate profile
};

struct AC_complex
{
    struct AC_token name;

    enum AC_complex_type type;
    union AC_complex_union value;
};

struct AC_complex* AC_complex_make();
void AC_complex_destroy(struct AC_complex* object);

void AC_complex_load(struct AC_complex* object, struct AC_scanner* scanner);
AC_ulong AC_complex_size(struct AC_complex* object, struct AC_program* program);

#endif /* TYPE_H_ */
