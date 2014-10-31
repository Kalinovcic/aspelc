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
 * File: scanner.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "scanner.h"

static struct AC_scanner* scanner;
static AC_byte currbyte;
static AC_uint currline = 1;

inline AC_bool isAlpha(AC_byte c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

inline AC_bool isDigit(AC_byte c)
{
    return c >= '0' && c <= '9';
}

inline AC_bool isNumberAllowed(AC_byte c)
{
    return c == 'i' || c == 'I' || c == 'l' || c == 'L' || c == 'u' || c == 'U' || c == 'f' || c == 'F' || c == 'd' || c == 'D';
}

inline AC_bool isOp(AC_byte c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '<' || c == '>' || c == '&' || c == '|' || c == '=' || c == '!';
}

inline AC_bool isAlnum(AC_byte c)
{
    return isAlpha(c) || isDigit(c);
}

inline AC_bool isWhite(AC_byte c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static void nextbyte()
{
    if(scanner->datac <= 0)
        currbyte = 0;
    else
    {
        currbyte = *scanner->datav;
        scanner->datav++;
        scanner->datac--;
    }
}

static void skipwhite()
{
    while(isWhite(currbyte))
    {
        if(currbyte == '\n')
            currline++;
        nextbyte();
    }
}

static struct AC_token readnumber()
{
    struct AC_token token;
    token.token = scanner->datav - 1;
    while(isDigit(currbyte) || currbyte == '.') nextbyte();
    while(isNumberAllowed(currbyte)) nextbyte();
    token.tokenl = scanner->datav - token.token - 1;
    token.line = currline;
    skipwhite();
    return token;
}

static struct AC_token readword()
{
    struct AC_token token;
    token.token = scanner->datav - 1;
    while(isAlnum(currbyte)) nextbyte();
    token.tokenl = scanner->datav - token.token - 1;
    token.line = currline;
    skipwhite();
    return token;
}

static struct AC_token readop()
{
    struct AC_token token;
    token.token = scanner->datav - 1;
    while(isOp(currbyte)) nextbyte();
    token.tokenl = scanner->datav - token.token - 1;
    token.line = currline;
    skipwhite();
    return token;
}

static struct AC_token readnext()
{
    if(isDigit(currbyte)) return readnumber();
    if(isAlpha(currbyte)) return readword();
    if(isOp(currbyte)) return readop();
    struct AC_token token;
    token.token = scanner->datav - 1;
    token.tokenl = 1;
    token.line = currline;
    nextbyte();
    skipwhite();
    return token;
}

static void read()
{
    nextbyte();
    skipwhite();
    while(currbyte != 0)
    {
        struct AC_token token = readnext();
        scanner->tokenc++;
        scanner->tokenv = realloc(scanner->tokenv, scanner->tokenc * sizeof(struct AC_token));
        scanner->tokenv[scanner->tokenc - 1] = token;
    }
}

struct AC_scanner* AC_scanner_make(AC_byte* data, AC_uint length)
{
    struct AC_scanner* object = malloc(sizeof(struct AC_scanner));
    object->datav = data;
    object->datac = length;

    object->tokenv = malloc(0);
    object->tokenc = 0;
    object->curr = 0;

    scanner = object;
    read();

    return object;
}

void AC_scanner_destroy(struct AC_scanner* object)
{
    free(object->tokenv);
    free(object);
}

struct AC_token AC_scanner_get(struct AC_scanner* object, AC_int off)
{
    assert((object->curr + off) >= 0);
    assert((object->curr + off) < object->tokenc);
    return object->tokenv[object->curr + off];
}

struct AC_token AC_scanner_getword(struct AC_scanner* object, AC_int off)
{
    struct AC_token token = AC_scanner_get(object, off);
    assert(isAlpha(*token.token));
    return token;
}

struct AC_token AC_scanner_getnumber(struct AC_scanner* object, AC_int off)
{
    struct AC_token token = AC_scanner_get(object, off);
    assert(isDigit(*token.token));
    return token;
}

struct AC_token AC_scanner_getop(struct AC_scanner* object, AC_int off)
{
    struct AC_token token = AC_scanner_get(object, off);
    assert(isOp(*token.token));
    return token;
}

AC_bool AC_scanner_isword(struct AC_scanner* object, AC_int off)
{
    return isAlpha(*AC_scanner_get(object, off).token);
}

AC_bool AC_scanner_isnumber(struct AC_scanner* object, AC_int off)
{
    return isDigit(*AC_scanner_get(object, off).token);
}

AC_bool AC_scanner_isop(struct AC_scanner* object, AC_int off)
{
    return isOp(*AC_scanner_get(object, off).token);
}

AC_uint AC_scanner_getpos(struct AC_scanner* object)
{
    return object->curr;
}

void AC_scanner_setpos(struct AC_scanner* object, AC_uint newpos)
{
    object->curr = newpos;
}

void AC_scanner_prev(struct AC_scanner* object)
{
    object->curr--;
}

void AC_scanner_next(struct AC_scanner* object)
{
    object->curr++;
    //printf("%.*s\n", object->tokenv[object->curr].tokenl, object->tokenv[object->curr].token);
}

void AC_scanner_match(struct AC_scanner* object, const char* cstr)
{
    assert(AC_token_compare_raw(AC_scanner_get(object, 0), cstr));
    AC_scanner_next(object);
}

AC_bool AC_scanner_eof(struct AC_scanner* object)
{
    if(object->curr == object->tokenc) return AC_TRUE;
    return AC_FALSE;
}
