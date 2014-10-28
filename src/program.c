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
 * File: program.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "program.h"

struct AC_namespace* AC_namespace_make()
{
    struct AC_namespace* object = malloc(sizeof(struct AC_namespace));
    object->typev_export = malloc(0);
    object->varv_export = malloc(0);
    object->funcv_export = malloc(0);
    object->childnsv_export = malloc(0);

    object->typev = malloc(0);
    object->varv = malloc(0);
    object->funcv = malloc(0);
    object->childnsv = malloc(0);

    object->typec_export = 0;
    object->varc_export = 0;
    object->funcc_export = 0;
    object->childnsc_export = 0;

    object->typec = 0;
    object->varc = 0;
    object->funcc = 0;
    object->childnsc = 0;
    return object;
}

void AC_namespace_destroy(struct AC_namespace* object)
{
    AC_uint i;
    for(i = 0; i < object->typec_export; i++)
        AC_type_destroy(object->typev_export[i]);
    for(i = 0; i < object->varc_export; i++)
        AC_variable_destroy(object->varv_export[i]);
    for(i = 0; i < object->funcc_export; i++)
        AC_function_destroy(object->funcv_export[i]);
    for(i = 0; i < object->childnsc_export; i++)
        AC_namespace_destroy(object->childnsv_export[i]);
    free(object->typev_export);
    free(object->varv_export);
    free(object->funcv_export);
    free(object->childnsv_export);

    for(i = 0; i < object->typec; i++)
        AC_type_destroy(object->typev[i]);
    for(i = 0; i < object->varc; i++)
        AC_variable_destroy(object->varv[i]);
    for(i = 0; i < object->funcc; i++)
        AC_function_destroy(object->funcv[i]);
    for(i = 0; i < object->childnsc; i++)
        AC_namespace_destroy(object->childnsv[i]);
    free(object->typev);
    free(object->varv);
    free(object->funcv);
    free(object->childnsv);

    free(object);
}

void* AC_namespace_push(struct AC_namespace* object, void*** targetv, AC_uint* targetc, AC_make make)
{
    (*targetc)++;
    *targetv = realloc(*targetv, *targetc * sizeof(void*));
    (*targetv)[(*targetc) - 1] = make();
    return (*targetv)[(*targetc) - 1];
}

void AC_namespace_load(struct AC_namespace* object, AC_bool incomplete, struct AC_scanner* scanner)
{
    if(incomplete == AC_FALSE)
    {
        AC_scanner_match(scanner, "namespace");
        struct AC_token token = AC_scanner_getword(scanner, 0);
        AC_scanner_next(scanner);
        object->name = token;
        AC_scanner_match(scanner, "{");
    }
    else
    {
        object->name.token = AC_NULL;
        object->name.tokenl = 0;
    }

    while(incomplete == AC_FALSE
            ? AC_token_compare_raw(AC_scanner_get(scanner, 0), "}") == AC_FALSE
            : AC_scanner_eof(scanner) == AC_FALSE)
    {
        struct AC_token token = AC_scanner_get(scanner, 0);

        struct AC_namespace*** tv_ns = &(object->childnsv);
        struct AC_function*** tv_func = &(object->funcv);
        struct AC_type*** tv_tdef = &(object->typev);
        AC_uint* tc_ns = &(object->childnsc);
        AC_uint* tc_func = &(object->funcc);
        AC_uint* tc_tdef = &(object->typec);

        if(AC_token_compare_raw(token, "export") == AC_TRUE)
        {
            AC_scanner_match(scanner, "export");
            token = AC_scanner_get(scanner, 0);

            tv_ns = &(object->childnsv_export);
            tv_func = &(object->funcv_export);
            tv_tdef = &(object->typev_export);
            tc_ns = &(object->childnsc_export);
            tc_func = &(object->funcc_export);
            tc_tdef = &(object->typec_export);
        }

        if(AC_token_compare_raw(token, "namespace") == AC_TRUE)
        {
            struct AC_namespace* newns = AC_namespace_push(object, (void***) tv_ns, tc_ns, (AC_make) &AC_namespace_make);
            newns->parent = object;
            AC_namespace_load(newns, AC_FALSE, scanner);
        }
        else if(AC_token_compare_raw(token, "function") == AC_TRUE)
        {
            struct AC_function* newfunc = AC_namespace_push(object, (void***) tv_func, tc_func, (AC_make) &AC_function_make);
            newfunc->ns = object;
            AC_function_load(newfunc, scanner);
        }
        else if(AC_token_compare_raw(token, "struct") == AC_TRUE)
        {
            struct AC_type* newtype = AC_namespace_push(object, (void***) tv_tdef, tc_tdef, (AC_make) &AC_type_make);
            AC_type_load(newtype, scanner);
        }
        else assert(0);
    }

    if(incomplete == AC_FALSE)
        AC_scanner_match(scanner, "}");
}

void AC_namespace_translate(struct AC_namespace* object, struct AC_output* output)
{
    AC_uint i;
    for(i = 0; i < object->funcc_export; i++)
        AC_function_translate(object->funcv_export[i], output);
    for(i = 0; i < object->funcc; i++)
        AC_function_translate(object->funcv[i], output);

    for(i = 0; i < object->childnsc_export; i++)
        AC_namespace_translate(object->childnsv_export[i], output);
    for(i = 0; i < object->childnsc; i++)
        AC_namespace_translate(object->childnsv[i], output);
}

struct AC_type* AC_namespace_findtype(struct AC_namespace* object, struct AC_identifier* identifier, AC_bool allowlocal)
{
    AC_uint i = 0;
    if(AC_identifier_hassub(identifier) == AC_TRUE)
    {
        for(; i < object->childnsc; i++)
            if(AC_token_compare(object->childnsv[i]->name, identifier->name))
                return AC_namespace_findtype(object->childnsv[i], identifier->sub, AC_FALSE);
    }
    else
    {
        for(; i < object->typec; i++)
            if(AC_token_compare(object->typev[i]->name, identifier->name))
                return object->typev[i];
    }
    return AC_NULL;
}

struct AC_function* AC_namespace_findfunc(struct AC_namespace* object, struct AC_identifier* identifier, AC_bool allowlocal)
{
    AC_uint i = 0;
    if(AC_identifier_hassub(identifier) == AC_TRUE)
    {
        for(; i < object->childnsc; i++)
            if(AC_token_compare(object->childnsv[i]->name, identifier->name))
                return AC_namespace_findfunc(object->childnsv[i], identifier->sub, AC_FALSE);
    }
    else
    {
        for(; i < object->funcc; i++)
            if(AC_token_compare(object->funcv[i]->name, identifier->name))
                return object->funcv[i];
    }
    return AC_NULL;
}

struct AC_program* AC_program_make(struct AC_scanner* scanner)
{
    struct AC_program* object = malloc(sizeof(struct AC_program));
    object->globalns = AC_namespace_make();
    object->globalns->parent = AC_NULL;

    AC_program_load(object, scanner);
    return object;
}

void AC_program_destroy(struct AC_program* object)
{
    AC_namespace_destroy(object->globalns);
    free(object);
}

void AC_program_load(struct AC_program* object, struct AC_scanner* scanner)
{
    AC_namespace_load(object->globalns, AC_TRUE, scanner);
}

void AC_program_translate(struct AC_program* object, struct AC_output* output)
{
    AC_namespace_translate(object->globalns, output);
}

struct AC_type* AC_program_findtype(struct AC_program* object, struct AC_identifier* identifier)
{
    return AC_namespace_findtype(object->globalns, identifier, AC_TRUE);
}

struct AC_function* AC_program_findfunc(struct AC_program* object, struct AC_identifier* identifier)
{
    return AC_namespace_findfunc(object->globalns, identifier, AC_TRUE);
}
