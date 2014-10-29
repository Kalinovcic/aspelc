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
 * File: statement.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "statement.h"

struct AC_statement* AC_statement_make()
{
    struct AC_statement* object = malloc(sizeof(struct AC_statement));
    return object;
}

void AC_statement_destroy(struct AC_statement* object)
{
    switch(object->type)
    {
    case AC_STATEMENT_SIMPLE: free(object->value.simple.tokenv); break;
    case AC_STATEMENT_IF:
        AC_expression_destroy(object->value.zif.condition);
        AC_block_destroy(object->value.zif.execblock);
        if(object->value.zif.elseblock != AC_NULL)
            AC_block_destroy(object->value.zif.elseblock);
        break;
    }
    free(object);
}

static void AC_statement_load_simple(struct AC_statement* object, struct AC_scanner* scanner, const char* rawend)
{
    object->type = AC_STATEMENT_SIMPLE;
    do
    {
        struct AC_token token = AC_scanner_get(scanner, 0);
        if(AC_token_compare_raw(token, rawend) == AC_TRUE) break;
        AC_scanner_next(scanner);
        object->value.simple.tokenc++;
        object->value.simple.tokenv = realloc(object->value.simple.tokenv, object->value.simple.tokenc * sizeof(struct AC_token));
        object->value.simple.tokenv[object->value.simple.tokenc - 1] = token;
    } while(1);
}

static void AC_statement_load_if(struct AC_statement* object, struct AC_scanner* scanner)
{
    object->type = AC_STATEMENT_IF;

    AC_scanner_match(scanner, "if");
    object->value.zif.condition = AC_expression_make();
    object->value.zif.execblock = AC_block_make();
    object->value.zif.elseblock = AC_NULL;
    AC_expression_load(object->value.zif.condition, scanner);
    AC_block_load(object->value.zif.execblock, scanner);
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "else") == AC_TRUE)
    {
        AC_scanner_match(scanner, "else");
        object->value.zif.elseblock = AC_block_make();
        AC_block_load(object->value.zif.elseblock, scanner);
    }
}

void AC_statement_load(struct AC_statement* object, struct AC_scanner* scanner, const char* rawend)
{
    struct AC_token token = AC_scanner_get(scanner, 0);
    if(AC_token_compare_raw(token, "if") == AC_TRUE)
        AC_statement_load_if(object, scanner);
    else
        AC_statement_load_simple(object, scanner, rawend);
}

static void AC_statement_translate_simple(struct AC_statement* object, struct AC_output* output, struct AC_program* program)
{

}

static void AC_statement_translate_if(struct AC_statement* object, struct AC_output* output, struct AC_program* program)
{
    AC_expression_translate(object->value.zif.condition, output, program);
    char* label1 = AC_label_newname();
    char* label2 = AC_NULL;
    if(object->value.zif.elseblock != AC_NULL)
        label2 = AC_label_newname();

    AC_output_write(output, "ifn %s", label1);
    AC_block_translate(object->value.zif.execblock, output, program);
    if(label2 != AC_NULL)
        AC_output_write(output, "goto %s", label2);
    AC_output_writeraw(output, "%s: \n", label1);
    if(label2 != AC_NULL)
    {
        AC_block_translate(object->value.zif.elseblock, output, program);
        AC_output_writeraw(output, "%s: \n", label2);
    }

    free(label1);
    if(label2 != AC_NULL)
        free(label2);
}

void AC_statement_translate(struct AC_statement* object, struct AC_output* output, struct AC_program* program)
{
    switch(object->type)
    {
    case AC_STATEMENT_SIMPLE: AC_statement_translate_simple(object, output, program); break;
    case AC_STATEMENT_IF: AC_statement_translate_if(object, output, program); break;
    }
}
