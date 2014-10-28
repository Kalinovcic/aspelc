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
 * File: scanner.h
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef SCANNER_H_
#define SCANNER_H_

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "acdef.h"
#include "token.h"

struct AC_scanner
{
    AC_byte* datav;
    AC_uint datac;

    struct AC_token* tokenv;
    AC_uint tokenc;
    AC_uint curr;
};

struct AC_scanner* AC_scanner_make(AC_byte* data, AC_uint length);
void AC_scanner_destroy(struct AC_scanner* object);

struct AC_token AC_scanner_get(struct AC_scanner* object, AC_int off);
struct AC_token AC_scanner_getword(struct AC_scanner* object, AC_int off);
struct AC_token AC_scanner_getnumber(struct AC_scanner* object, AC_int off);
struct AC_token AC_scanner_getop(struct AC_scanner* object, AC_int off);
AC_bool AC_scanner_isword(struct AC_scanner* object, AC_int off);
AC_bool AC_scanner_isnumber(struct AC_scanner* object, AC_int off);
AC_bool AC_scanner_isop(struct AC_scanner* object, AC_int off);
void AC_scanner_prev(struct AC_scanner* object);
void AC_scanner_next(struct AC_scanner* object);
void AC_scanner_match(struct AC_scanner* object, const char* cstr);
AC_bool AC_scanner_eof(struct AC_scanner* object);

#endif /* SCANNER_H_ */
