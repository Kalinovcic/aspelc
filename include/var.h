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
 * File: var.h
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef VAR_H_
#define VAR_H_

#include "scanner.h"
#include "token.h"
#include "type.h"

struct AC_variable
{
    struct AC_token name;
    struct AC_typename* typename;
};

struct AC_variable* AC_variable_make();
void AC_variable_destroy(struct AC_variable* object);

void AC_variable_load(struct AC_variable* object, struct AC_scanner* scanner);

#endif /* VAR_H_ */
