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
 * File: translator_typecheck.cpp
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

bool TranslatorA11::isInteger(Type type)
{
    return type == INT || type == LONG;
}

void TranslatorA11::checkInteger(Type type, std::string op)
{
    if(!isInteger(type))
    {
        std::string name = getTypeName(type);
        abortnl("invalid operand of type '" + name + "' to operator '" + op + "'");
    }
}

void TranslatorA11::checkInteger2(Type type1, Type type2, std::string op)
{
    if(!isInteger(type1) || !isInteger(type2))
    {
        std::string name1 = getTypeName(type1);
        std::string name2 = getTypeName(type2);
        abortnl("invalid operands of types '" + name1 + "' and '" + name2 + "' to operator '" + op + "'");
    }
}
