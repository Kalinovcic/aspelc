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
 * File: translator_input.cpp
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

std::string AspelTranslator::nextToken()
{
    m_token = m_scanner.scan();
    return m_token;
}

std::string AspelTranslator::getName()
{
    if(!isAlpha(m_token[0])) expected("name");
    std::string name = m_token;
    nextToken();
    return name;
}

std::string AspelTranslator::getNumber()
{
    std::string number = m_token;
    if(!isDigit(number[0]))
        expected("number");
    nextToken();
    return number;
}

void AspelTranslator::match(std::string x)
{
    if(m_token != x) expected("'" + x + "'");
    nextToken();
}
