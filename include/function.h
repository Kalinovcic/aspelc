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
 * File: function.h
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include "acdef.h"
#include "output.h"
#include "program.h"
#include "token.h"
#include "type.h"
#include "scanner.h"
#include "statement.h"

struct AC_blockvar
{
    struct AC_token name;
    AC_ulong memloc;
};

struct AC_block
{
    struct AC_statement** statementv;
    AC_uint statementc;
};

struct AC_block* AC_block_make();
void AC_block_destroy(struct AC_block* object);

struct AC_statement* AC_block_pushstatement(struct AC_block* object);

void AC_block_load(struct AC_block* object, struct AC_scanner* scanner);
void AC_block_translate(struct AC_block* object, struct AC_output* output, struct AC_program* program);

struct AC_function
{
    struct AC_namespace* ns;

    struct AC_token name;
    struct AC_typename* rtype;

    struct AC_block* funcblock;
};

struct AC_function* AC_function_make();
void AC_function_destroy(struct AC_function* object);

void AC_function_load(struct AC_function* object, struct AC_scanner* scanner);
void AC_function_translate(struct AC_function* object, struct AC_output* output, struct AC_program* program);
void AC_function_writensname(struct AC_function* object, struct AC_output* output);

#endif /* FUNCTION_H_ */
