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
 * File: token.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "token.h"

AC_bool AC_token_compare_raw(struct AC_token token, const char* cstr)
{
    AC_uint len = strlen(cstr);
    if(len != token.tokenl) return AC_FALSE;
    if(strncmp(cstr, (const char*) token.token, token.tokenl)) return AC_FALSE;
    return AC_TRUE;
}

AC_bool AC_token_compare(struct AC_token token, struct AC_token token2)
{
    if(token.tokenl != token2.tokenl) return AC_FALSE;
    if(strncmp((const char*) token.token, (const char*) token2.token, token.tokenl)) return AC_FALSE;
    return AC_TRUE;
}
