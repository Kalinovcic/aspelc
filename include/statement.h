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
 * File: statement.h
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef STATEMENT_H_
#define STATEMENT_H_

#include <stdlib.h>

#include "acdef.h"
#include "expression.h"
#include "function.h"
#include "label.h"
#include "output.h"
#include "scanner.h"
#include "token.h"

enum AC_statement_type
{
    AC_STATEMENT_SIMPLE,
    AC_STATEMENT_IF,
};

struct AC_statement_simple
{
    struct AC_token* tokenv;
    AC_uint tokenc;
};

struct AC_statement_if
{
    struct AC_expression* condition;
    struct AC_block* execblock;
    struct AC_block* elseblock;
};

union AC_statement_value
{
    struct AC_statement_simple simple;
    struct AC_statement_if zif;
};

struct AC_statement
{
    enum AC_statement_type type;
    union AC_statement_value value;
};

struct AC_statement* AC_statement_make();
void AC_statement_destroy(struct AC_statement* object);
void AC_statement_load(struct AC_statement* object, struct AC_scanner* scanner, const char* rawend);
void AC_statement_translate(struct AC_statement* object, struct AC_output* output);

#endif /* STATEMENT_H_ */
