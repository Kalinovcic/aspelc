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
 * File: identifier.h
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "acdef.h"
#include "token.h"
#include "scanner.h"

struct AC_identifier
{
    struct AC_token name;
    struct AC_identifier* sub;
};

struct AC_identifier* AC_identifier_make();
void AC_identifier_destroy(struct AC_identifier* object);

AC_bool AC_identifier_hassub(struct AC_identifier* object);
void AC_identifier_setsub(struct AC_identifier* object, struct AC_identifier* sub);
struct AC_identifier* AC_identifier_getsub(struct AC_identifier* object);

void AC_identifier_load(struct AC_identifier* object, struct AC_scanner* scanner);

void AC_identifier_print(struct AC_identifier* object);

#endif /* IDENTIFIER_H_ */
