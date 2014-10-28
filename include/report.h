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
 * File: report.h
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef REPORT_H_
#define REPORT_H_

#include <stdlib.h>

#include "type.h"

#define AC_kill()       exit(1)

void AC_report(const char* format, ...);
void AC_report_abort();
void AC_report_header();

void AC_internal_error(const char* msg);

void AC_invalid_operand(struct AC_typename* type, const char* operator, AC_uint srcline);
void AC_invalid_conversion(struct AC_typename* from, struct AC_typename* to, AC_uint srcline);

#endif /* REPORT_H_ */
