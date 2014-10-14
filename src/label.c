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
 * File: label.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "label.h"

static AC_uint label_namec = 0;

char* AC_label_newname()
{
    char* labelname = malloc(12);
    sprintf(labelname, "%c%.10d%c", 'l', label_namec, 0);
    label_namec++;
    return labelname;
}
