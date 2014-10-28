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
 * File: codegen.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "codegen.h"

void AC_codegen_swap(AC_uint top, AC_uint bottom, struct AC_output* output)
{
    if(top != 4 && top != 8) AC_internal_error("invalid type size [codegen swap: top]\n");
    if(bottom != 4 && bottom != 8) AC_internal_error("invalid type size [codegen swap: bottom]\n");
    if(top == bottom) AC_output_write(output, "swap%d", top);
    else AC_output_write(output, "swap%d%d", top, bottom);
}
