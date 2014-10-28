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
 * File: report.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "report.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void AC_report(const char* format, ...)
{
    va_list arg;
    va_start(arg, format);
    vfprintf(stderr, format, arg);
    va_end(arg);
}

void AC_report_abort()
{
    AC_report("abort\n");
    AC_kill();
}

void AC_report_header()
{
    AC_report("aspelc report: ");
}

void AC_internal_error(const char* msg)
{
    AC_report("aspelc: internal error: FATAL: %s\n", msg);
    AC_report_abort();
}

void AC_invalid_operand(struct AC_typename* type, const char* operator, AC_uint srcline)
{
    AC_report_header();
    AC_report("invalid operand of type \"");
    AC_typename_print(type);
    AC_report("\" to %s near line %d\n", operator, srcline);
    AC_report_abort();
}


void AC_invalid_conversion(struct AC_typename* from, struct AC_typename* to, AC_uint srcline)
{
    AC_report_header();
    AC_report("invalid type conversion \"");
    AC_typename_print(from);
    AC_report("\"->\"");
    AC_typename_print(to);
    AC_report("\" near line %d\n", srcline);
    AC_report_abort();
}
