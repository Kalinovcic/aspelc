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
 * File: translator_codegen.cpp
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

std::string AspelTranslator::newLabel()
{
    return "l" + toString(m_labelCounter++);
}

void AspelTranslator::writeLabel(std::string labelname)
{
    writeln(labelname + ":");
}

void AspelTranslator::callFunction(std::string name)
{
    match("(");
    int paramc = 0;
    while(m_token != ")")
    {
        if(paramc) match(",");
        paramc++;
        boolExpression();
    }
    match(")");
    writeln("call " + name + " " + toString(paramc));
}
