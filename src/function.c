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
 * File: function.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "function.h"

struct AC_block* AC_block_make()
{
    struct AC_block* object = malloc(sizeof(struct AC_block));
    object->statementc = 0;
    object->statementv = malloc(0);
    return object;
}

void AC_block_destroy(struct AC_block* object)
{
    AC_uint i;
    for(i = 0; i < object->statementc; i++)
        AC_statement_destroy(object->statementv[i]);
    free(object->statementv);
    free(object);
}

struct AC_statement* AC_block_pushstatement(struct AC_block* object)
{
    object->statementc++;
    object->statementv = realloc(object->statementv, object->statementc * sizeof(void*));
    object->statementv[object->statementc - 1] = AC_statement_make();
    return object->statementv[object->statementc - 1];
}

void AC_block_load(struct AC_block* object, struct AC_scanner* scanner)
{
    AC_scanner_match(scanner, "{");
    do
    {
        struct AC_token token = AC_scanner_get(scanner, 0);
        if(AC_token_compare_raw(token, "}") == AC_TRUE) break;

        struct AC_statement* statement = AC_block_pushstatement(object);
        AC_statement_load(statement,  scanner);
    } while(1);

    AC_scanner_match(scanner, "}");
}

void AC_block_translate(struct AC_block* object, struct AC_output* output, struct AC_program* program)
{
    AC_uint i = 0;
    for(; i < object->statementc; i++)
        AC_statement_translate(object->statementv[i], output, program);
}

struct AC_function* AC_function_make()
{
    struct AC_function* object = malloc(sizeof(struct AC_function));
    object->rtype = AC_typename_make();
    object->funcblock = AC_block_make();
    return object;
}

void AC_function_destroy(struct AC_function* object)
{
    AC_typename_destroy(object->rtype);
    AC_block_destroy(object->funcblock);
    free(object);
}

static void AC_function_load_paramlist(struct AC_function* object, struct AC_scanner* scanner)
{
    AC_scanner_match(scanner, "(");
    AC_scanner_match(scanner, ")");
}

void AC_function_load(struct AC_function* object, struct AC_scanner* scanner)
{
    AC_scanner_match(scanner, "function");
    AC_typename_load(object->rtype, scanner);
    object->name = AC_scanner_getword(scanner, 0);
    AC_scanner_next(scanner);

    AC_function_load_paramlist(object, scanner);
    AC_block_load(object->funcblock, scanner);
}

void AC_function_translate(struct AC_function* object, struct AC_output* output, struct AC_program* program)
{
    AC_output_writeraw(output, "f: ");
    AC_function_writensname(object, output);
    AC_output_writeraw(output, "\n");
    AC_block_translate(object->funcblock, output, program);
}

void AC_function_writensname(struct AC_function* object, struct AC_output* output)
{
    AC_output_writeraw(output, "%.*s", object->name.tokenl, object->name.token);
    struct AC_namespace* ns = object->ns;
    while(ns->name.tokenl != 0)
    {
        AC_output_writeraw(output, "$%.*s", ns->name.tokenl, ns->name.token);
        ns = ns->parent;
    }
}
