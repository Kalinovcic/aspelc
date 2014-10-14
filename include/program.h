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
 * File: program.h
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

struct AC_program;

#include <stdlib.h>
#include <stdio.h>

#include "acdef.h"
#include "identifier.h"
#include "function.h"
#include "token.h"
#include "type.h"
#include "scanner.h"
#include "var.h"

struct AC_namespace
{
    struct AC_namespace* parent;
    struct AC_token name;

    struct AC_type** typev_export;
    struct AC_variable** varv_export;
    struct AC_function** funcv_export;
    struct AC_namespace** childnsv_export;

    struct AC_type** typev;
    struct AC_variable** varv;
    struct AC_function** funcv;
    struct AC_namespace** childnsv;

    AC_uint typec_export;
    AC_uint varc_export;
    AC_uint funcc_export;
    AC_uint childnsc_export;

    AC_uint typec;
    AC_uint varc;
    AC_uint funcc;
    AC_uint childnsc;
};

struct AC_program
{
    struct AC_namespace* globalns;
};

#include "struct.h"

struct AC_namespace* AC_namespace_make();
void AC_namespace_destroy(struct AC_namespace* object);

void* AC_namespace_push(struct AC_namespace* object, void*** targetv, AC_uint* targetc, AC_make make);

void AC_namespace_load(struct AC_namespace* object, AC_bool incomplete, struct AC_scanner* scanner);
void AC_namespace_translate(struct AC_namespace* object, struct AC_output* output);

struct AC_type* AC_namespace_findtype(struct AC_namespace* object, struct AC_identifier* identifier, AC_bool allowlocal);
struct AC_function* AC_namespace_findfunc(struct AC_namespace* object, struct AC_identifier* identifier, AC_bool allowlocal);

struct AC_program* AC_program_make(struct AC_scanner* scanner);
void AC_program_destroy(struct AC_program* object);

void AC_program_load(struct AC_program* object, struct AC_scanner* scanner);
void AC_program_translate(struct AC_program* object, struct AC_output* output);

struct AC_type* AC_program_findtype(struct AC_program* object, struct AC_identifier* identifier);
struct AC_function* AC_program_findfunc(struct AC_program* object, struct AC_identifier* identifier);

#endif /* PROGRAM_H_ */
