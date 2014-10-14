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
 * File: expression.h
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <stdlib.h>

#include "acdef.h"
#include "output.h"
#include "scanner.h"
#include "token.h"
#include "type.h"

enum AC_expr_const_type
{
    AC_EXPR_CONST_INT,
    AC_EXPR_CONST_FLOAT,
    AC_EXPR_CONST_LONG,
    AC_EXPR_CONST_DOUBLE,
};

struct AC_expr_const
{
    enum AC_expr_const_type type;
    struct AC_token value;
};

enum AC_expr_var_type
{
    AC_EXPR_VAR_VALUE,
    AC_EXPR_VAR_POINTER,
};

struct AC_expr_var
{
    enum AC_expr_var_type type;
    struct AC_token name;
};

struct AC_expr_new
{
    struct AC_typename* type;
    struct AC_token size;
};

struct AC_expr_size
{
    struct AC_typename* type;
};

enum AC_expr_level1_type
{
    AC_EXPR_LEVEL1_CONST,
    AC_EXPR_LEVEL1_VAR,
    AC_EXPR_LEVEL1_NEW,
    AC_EXPR_LEVEL1_SIZE,
    AC_EXPR_LEVEL1_EXPRESSION,
};

union AC_expr_level1_union
{
    struct AC_expr_const zconst;
    struct AC_expr_var var;
    struct AC_expr_new new;
    struct AC_expr_size size;
    struct AC_expr_expression* exprssion;
};

struct AC_expr_level1
{
    enum AC_expr_level1_type type;
    union AC_expr_level1_union value;
};

enum AC_expr_level2_type
{
    AC_EXPR_LEVEL2_NOP,
    AC_EXPR_LEVEL2_NEGATE,
    AC_EXPR_LEVEL2_LNOT,
    AC_EXPR_LEVEL2_BNOT,
};

struct AC_expr_level2
{
    enum AC_expr_level2_type type;
    struct AC_expr_level1 value;
};

enum AC_expr_level3_type
{
    AC_EXPR_LEVEL3_NOP,
    AC_EXPR_LEVEL3_CAST,
};

union AC_expr_level3_meta
{
    struct AC_typename* casttype;
};

struct AC_expr_level3
{
    enum AC_expr_level3_type type;
    struct AC_expr_level2 value;
    union AC_expr_level3_meta meta;
};

enum AC_expr_level4_type
{
    AC_EXPR_LEVEL4_NOP,
    AC_EXPR_LEVEL4_DEREFERENCE,
};

struct AC_expr_level4
{
    enum AC_expr_level4_type type;
    struct AC_expr_level3 value;
};

enum AC_expr_level5_type
{
    AC_EXPR_LEVEL5_NOP,
    AC_EXPR_LEVEL5_MULTIPLY,
    AC_EXPR_LEVEL5_DIVIDE,
    AC_EXPR_LEVEL5_REMAINDER,
};

struct AC_expr_level5
{
    enum AC_expr_level5_type type;
    struct AC_expr_level4 low;
    struct AC_expr_level5* next;
};

enum AC_expr_level6_type
{
    AC_EXPR_LEVEL6_NOP,
    AC_EXPR_LEVEL6_ADD,
    AC_EXPR_LEVEL6_SUBTRACT,
};

struct AC_expr_level6
{
    enum AC_expr_level6_type type;
    struct AC_expr_level5 low;
    struct AC_expr_level6* next;
};

enum AC_expr_level7_type
{
    AC_EXPR_LEVEL7_NOP,
    AC_EXPR_LEVEL7_SHIFTLEFT,
    AC_EXPR_LEVEL7_SHIFTRIGHT,
};

struct AC_expr_level7
{
    enum AC_expr_level7_type type;
    struct AC_expr_level6 low;
    struct AC_expr_level7* next;
};

enum AC_expr_level8_type
{
    AC_EXPR_LEVEL8_NOP,
    AC_EXPR_LEVEL8_LESS,
    AC_EXPR_LEVEL8_LESSEQUAL,
    AC_EXPR_LEVEL8_GREATER,
    AC_EXPR_LEVEL8_GREATEREQUAL,
};

struct AC_expr_level8
{
    enum AC_expr_level8_type type;
    struct AC_expr_level7 low;
    struct AC_expr_level8* next;
};

enum AC_expr_level9_type
{
    AC_EXPR_LEVEL9_NOP,
    AC_EXPR_LEVEL9_EQUAL,
    AC_EXPR_LEVEL9_NOTEQUAL,
};

struct AC_expr_level9
{
    enum AC_expr_level9_type type;
    struct AC_expr_level8 low;
    struct AC_expr_level9* next;
};

enum AC_expr_level10_type
{
    AC_EXPR_LEVEL10_NOP,
    AC_EXPR_LEVEL10_BAND,
};

struct AC_expr_level10
{
    enum AC_expr_level10_type type;
    struct AC_expr_level9 low;
    struct AC_expr_level10* next;
};

enum AC_expr_level11_type
{
    AC_EXPR_LEVEL11_NOP,
    AC_EXPR_LEVEL11_BXOR,
};

struct AC_expr_level11
{
    enum AC_expr_level11_type type;
    struct AC_expr_level10 low;
    struct AC_expr_level11* next;
};

enum AC_expr_level12_type
{
    AC_EXPR_LEVEL12_NOP,
    AC_EXPR_LEVEL12_BOR,
};

struct AC_expr_level12
{
    enum AC_expr_level12_type type;
    struct AC_expr_level11 low;
    struct AC_expr_level12* next;
};

enum AC_expr_level13_type
{
    AC_EXPR_LEVEL13_NOP,
    AC_EXPR_LEVEL13_LAND,
};

struct AC_expr_level13
{
    enum AC_expr_level13_type type;
    struct AC_expr_level12 low;
    struct AC_expr_level13* next;
};

enum AC_expr_level14_type
{
    AC_EXPR_LEVEL14_NOP,
    AC_EXPR_LEVEL14_LOR,
};

struct AC_expr_level14
{
    enum AC_expr_level14_type type;
    struct AC_expr_level13 low;
    struct AC_expr_level14* next;
};

struct AC_expression
{
    struct AC_expr_level14 top;
};

void AC_expr_level1_make(struct AC_expr_level1* object, struct AC_scanner* scanner);
void AC_expr_level2_make(struct AC_expr_level2* object, struct AC_scanner* scanner);
void AC_expr_level3_make(struct AC_expr_level3* object, struct AC_scanner* scanner);
void AC_expr_level4_make(struct AC_expr_level4* object, struct AC_scanner* scanner);
void AC_expr_level5_make(struct AC_expr_level5* object, struct AC_scanner* scanner);
void AC_expr_level6_make(struct AC_expr_level6* object, struct AC_scanner* scanner);
void AC_expr_level7_make(struct AC_expr_level7* object, struct AC_scanner* scanner);
void AC_expr_level8_make(struct AC_expr_level8* object, struct AC_scanner* scanner);
void AC_expr_level9_make(struct AC_expr_level9* object, struct AC_scanner* scanner);
void AC_expr_level10_make(struct AC_expr_level10* object, struct AC_scanner* scanner);
void AC_expr_level11_make(struct AC_expr_level11* object, struct AC_scanner* scanner);
void AC_expr_level12_make(struct AC_expr_level12* object, struct AC_scanner* scanner);
void AC_expr_level13_make(struct AC_expr_level13* object, struct AC_scanner* scanner);
void AC_expr_level14_make(struct AC_expr_level14* object, struct AC_scanner* scanner);

void AC_expr_level1_destroy(struct AC_expr_level1* object);
void AC_expr_level2_destroy(struct AC_expr_level2* object);
void AC_expr_level3_destroy(struct AC_expr_level3* object);
void AC_expr_level4_destroy(struct AC_expr_level4* object);
void AC_expr_level5_destroy(struct AC_expr_level5* object);
void AC_expr_level6_destroy(struct AC_expr_level6* object);
void AC_expr_level7_destroy(struct AC_expr_level7* object);
void AC_expr_level8_destroy(struct AC_expr_level8* object);
void AC_expr_level9_destroy(struct AC_expr_level9* object);
void AC_expr_level10_destroy(struct AC_expr_level10* object);
void AC_expr_level11_destroy(struct AC_expr_level11* object);
void AC_expr_level12_destroy(struct AC_expr_level12* object);
void AC_expr_level13_destroy(struct AC_expr_level13* object);
void AC_expr_level14_destroy(struct AC_expr_level14* object);

struct AC_expression* AC_expression_make();
void AC_expression_destroy(struct AC_expression* object);

void AC_expression_load(struct AC_expression* object, struct AC_scanner* scanner);
void AC_expression_translate(struct AC_expression* object, struct AC_output* output);

#endif /* EXPRESSION_H_ */
