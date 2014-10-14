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
 * File: expression.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "expression.h"

void AC_expr_level1_make(struct AC_expr_level1* object, struct AC_scanner* scanner)
{
    AC_scanner_next(scanner);
}

void AC_expr_level2_make(struct AC_expr_level2* object, struct AC_scanner* scanner)
{
    AC_expr_level1_make(&object->value, scanner);
    object->type = AC_EXPR_LEVEL2_NOP;
    AC_bool isneg = AC_token_compare_raw(AC_scanner_get(scanner, 0), "-");
    AC_bool islnot = AC_token_compare_raw(AC_scanner_get(scanner, 0), "!");
    AC_bool isbnot = AC_token_compare_raw(AC_scanner_get(scanner, 0), "~");
    if(isneg == AC_TRUE) object->type = AC_EXPR_LEVEL2_NEGATE;
    if(islnot == AC_TRUE) object->type = AC_EXPR_LEVEL2_LNOT;
    if(isbnot == AC_TRUE) object->type = AC_EXPR_LEVEL2_BNOT;
}

void AC_expr_level3_make(struct AC_expr_level3* object, struct AC_scanner* scanner)
{
    AC_expr_level2_make(&object->value, scanner);
    object->type = AC_EXPR_LEVEL3_NOP;
}

void AC_expr_level4_make(struct AC_expr_level4* object, struct AC_scanner* scanner)
{
    object->type = AC_EXPR_LEVEL4_NOP;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "*"))
    {
        AC_scanner_match(scanner, "*");
        object->type = AC_EXPR_LEVEL4_DEREFERENCE;
    }
    AC_expr_level3_make(&object->value, scanner);
}

void AC_expr_level5_make(struct AC_expr_level5* object, struct AC_scanner* scanner)
{
    AC_expr_level4_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL5_NOP;
    object->next = AC_NULL;
    AC_bool ismul = AC_token_compare_raw(AC_scanner_get(scanner, 0), "*");
    AC_bool isdiv = AC_token_compare_raw(AC_scanner_get(scanner, 0), "/");
    AC_bool isrem = AC_token_compare_raw(AC_scanner_get(scanner, 0), "%");
    if(ismul == AC_TRUE || isdiv == AC_TRUE || isrem == AC_TRUE)
    {
        if(ismul)
        {
            object->type = AC_EXPR_LEVEL5_MULTIPLY;
            AC_scanner_match(scanner, "+");
        }
        if(isdiv)
        {
            object->type = AC_EXPR_LEVEL5_DIVIDE;
            AC_scanner_match(scanner, "-");
        }
        if(isrem)
        {
            object->type = AC_EXPR_LEVEL5_REMAINDER;
            AC_scanner_match(scanner, "%");
        }
        object->next = malloc(sizeof(struct AC_expr_level5));
        AC_expr_level5_make(object->next, scanner);
    }
}

void AC_expr_level6_make(struct AC_expr_level6* object, struct AC_scanner* scanner)
{
    AC_expr_level5_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL6_NOP;
    object->next = AC_NULL;
    AC_bool isadd = AC_token_compare_raw(AC_scanner_get(scanner, 0), "+");
    AC_bool issub = AC_token_compare_raw(AC_scanner_get(scanner, 0), "-");
    if(isadd == AC_TRUE || issub == AC_TRUE)
    {
        if(isadd)
        {
            object->type = AC_EXPR_LEVEL6_ADD;
            AC_scanner_match(scanner, "+");
        }
        if(issub)
        {
            object->type = AC_EXPR_LEVEL6_SUBTRACT;
            AC_scanner_match(scanner, "-");
        }
        object->next = malloc(sizeof(struct AC_expr_level6));
        AC_expr_level6_make(object->next, scanner);
    }
}

void AC_expr_level7_make(struct AC_expr_level7* object, struct AC_scanner* scanner)
{
    AC_expr_level6_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL7_NOP;
    object->next = AC_NULL;
    AC_bool islsft = AC_token_compare_raw(AC_scanner_get(scanner, 0), "<<");
    AC_bool isrsft = AC_token_compare_raw(AC_scanner_get(scanner, 0), ">>");
    if(islsft == AC_TRUE || isrsft == AC_TRUE)
    {
        if(islsft)
        {
            object->type = AC_EXPR_LEVEL7_SHIFTLEFT;
            AC_scanner_match(scanner, "<<");
        }
        if(isrsft)
        {
            object->type = AC_EXPR_LEVEL7_SHIFTRIGHT;
            AC_scanner_match(scanner, ">>");
        }
        object->next = malloc(sizeof(struct AC_expr_level7));
        AC_expr_level7_make(object->next, scanner);
    }
}

void AC_expr_level8_make(struct AC_expr_level8* object, struct AC_scanner* scanner)
{
    AC_expr_level7_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL8_NOP;
    object->next = AC_NULL;
    AC_bool islt = AC_token_compare_raw(AC_scanner_get(scanner, 0), "<");
    AC_bool isle = AC_token_compare_raw(AC_scanner_get(scanner, 0), "<=");
    AC_bool isgt = AC_token_compare_raw(AC_scanner_get(scanner, 0), ">");
    AC_bool isge = AC_token_compare_raw(AC_scanner_get(scanner, 0), ">=");
    if(islt == AC_TRUE || isle == AC_TRUE || isgt == AC_TRUE || isge == AC_TRUE)
    {
        if(islt)
        {
            object->type = AC_EXPR_LEVEL8_LESS;
            AC_scanner_match(scanner, "<");
        }
        if(isle)
        {
            object->type = AC_EXPR_LEVEL8_LESSEQUAL;
            AC_scanner_match(scanner, "<=");
        }
        if(isgt)
        {
            object->type = AC_EXPR_LEVEL8_GREATER;
            AC_scanner_match(scanner, ">");
        }
        if(isge)
        {
            object->type = AC_EXPR_LEVEL8_GREATEREQUAL;
            AC_scanner_match(scanner, ">=");
        }
        object->next = malloc(sizeof(struct AC_expr_level8));
        AC_expr_level8_make(object->next, scanner);
    }
}

void AC_expr_level9_make(struct AC_expr_level9* object, struct AC_scanner* scanner)
{
    AC_expr_level8_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL9_NOP;
    object->next = AC_NULL;
    AC_bool iseql = AC_token_compare_raw(AC_scanner_get(scanner, 0), "==");
    AC_bool isneql = AC_token_compare_raw(AC_scanner_get(scanner, 0), "!=");
    if(iseql == AC_TRUE || isneql == AC_TRUE)
    {
        if(iseql)
        {
            object->type = AC_EXPR_LEVEL9_EQUAL;
            AC_scanner_match(scanner, "==");
        }
        if(isneql)
        {
            object->type = AC_EXPR_LEVEL9_NOTEQUAL;
            AC_scanner_match(scanner, "!=");
        }
        object->next = malloc(sizeof(struct AC_expr_level9));
        AC_expr_level9_make(object->next, scanner);
    }
}

void AC_expr_level10_make(struct AC_expr_level10* object, struct AC_scanner* scanner)
{
    AC_expr_level9_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL10_NOP;
    object->next = AC_NULL;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "&") == AC_TRUE)
    {
        object->type = AC_EXPR_LEVEL10_BAND;
        object->next = malloc(sizeof(struct AC_expr_level10));
        AC_scanner_match(scanner, "&");
        AC_expr_level10_make(object->next, scanner);
    }
}

void AC_expr_level11_make(struct AC_expr_level11* object, struct AC_scanner* scanner)
{
    AC_expr_level10_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL11_NOP;
    object->next = AC_NULL;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "^") == AC_TRUE)
    {
        object->type = AC_EXPR_LEVEL11_BXOR;
        object->next = malloc(sizeof(struct AC_expr_level11));
        AC_scanner_match(scanner, "^");
        AC_expr_level11_make(object->next, scanner);
    }
}

void AC_expr_level12_make(struct AC_expr_level12* object, struct AC_scanner* scanner)
{
    AC_expr_level11_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL12_NOP;
    object->next = AC_NULL;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "|") == AC_TRUE)
    {
        object->type = AC_EXPR_LEVEL12_BOR;
        object->next = malloc(sizeof(struct AC_expr_level12));
        AC_scanner_match(scanner, "|");
        AC_expr_level12_make(object->next, scanner);
    }
}

void AC_expr_level13_make(struct AC_expr_level13* object, struct AC_scanner* scanner)
{
    AC_expr_level12_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL13_NOP;
    object->next = AC_NULL;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "&&") == AC_TRUE)
    {
        object->type = AC_EXPR_LEVEL13_LAND;
        object->next = malloc(sizeof(struct AC_expr_level13));
        AC_scanner_match(scanner, "&&");
        AC_expr_level13_make(object->next, scanner);
    }
}

void AC_expr_level14_make(struct AC_expr_level14* object, struct AC_scanner* scanner)
{
    AC_expr_level13_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL14_NOP;
    object->next = AC_NULL;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "||") == AC_TRUE)
    {
        object->type = AC_EXPR_LEVEL14_LOR;
        object->next = malloc(sizeof(struct AC_expr_level14));
        AC_scanner_match(scanner, "||");
        AC_expr_level14_make(object->next, scanner);
    }
}

void AC_expr_level1_destroy(struct AC_expr_level1* object)
{

}

void AC_expr_level2_destroy(struct AC_expr_level2* object)
{
    AC_expr_level1_destroy(&object->value);
}

void AC_expr_level3_destroy(struct AC_expr_level3* object)
{
    AC_expr_level2_destroy(&object->value);
}

void AC_expr_level4_destroy(struct AC_expr_level4* object)
{
    AC_expr_level3_destroy(&object->value);
}

void AC_expr_level5_destroy(struct AC_expr_level5* object)
{
    AC_expr_level4_destroy(&object->low);
    if(object->type != AC_EXPR_LEVEL5_NOP)
    {
        AC_expr_level5_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level6_destroy(struct AC_expr_level6* object)
{
    AC_expr_level5_destroy(&object->low);
    if(object->type != AC_EXPR_LEVEL6_NOP)
    {
        AC_expr_level6_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level7_destroy(struct AC_expr_level7* object)
{
    AC_expr_level6_destroy(&object->low);
    if(object->type != AC_EXPR_LEVEL7_NOP)
    {
        AC_expr_level7_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level8_destroy(struct AC_expr_level8* object)
{
    AC_expr_level7_destroy(&object->low);
    if(object->type != AC_EXPR_LEVEL8_NOP)
    {
        AC_expr_level8_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level9_destroy(struct AC_expr_level9* object)
{
    AC_expr_level8_destroy(&object->low);
    if(object->type != AC_EXPR_LEVEL9_NOP)
    {
        AC_expr_level9_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level10_destroy(struct AC_expr_level10* object)
{
    AC_expr_level9_destroy(&object->low);
    if(object->type == AC_EXPR_LEVEL10_BAND)
    {
        AC_expr_level10_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level11_destroy(struct AC_expr_level11* object)
{
    AC_expr_level10_destroy(&object->low);
    if(object->type == AC_EXPR_LEVEL11_BXOR)
    {
        AC_expr_level11_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level12_destroy(struct AC_expr_level12* object)
{
    AC_expr_level11_destroy(&object->low);
    if(object->type == AC_EXPR_LEVEL12_BOR)
    {
        AC_expr_level12_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level13_destroy(struct AC_expr_level13* object)
{
    AC_expr_level12_destroy(&object->low);
    if(object->type == AC_EXPR_LEVEL13_LAND)
    {
        AC_expr_level13_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level14_destroy(struct AC_expr_level14* object)
{
    AC_expr_level13_destroy(&object->low);
    if(object->type == AC_EXPR_LEVEL14_LOR)
    {
        AC_expr_level14_destroy(object->next);
        free(object->next);
    }
}

struct AC_expression* AC_expression_make()
{
    struct AC_expression* object = malloc(sizeof(struct AC_expression));
    return object;
}

void AC_expression_destroy(struct AC_expression* object)
{
    AC_expr_level14_destroy(&object->top);
    free(object);
}

void AC_expression_load(struct AC_expression* object, struct AC_scanner* scanner)
{
    AC_expr_level14_make(&object->top, scanner);
}

void AC_expression_translate(struct AC_expression* object, struct AC_output* output)
{

}
