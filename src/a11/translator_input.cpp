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
 * Description: Methods from AspelTranslator for getting input from a LexicalScanner,
 *              checking a tokens type and matching.
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

std::string TranslatorA11::nextToken()
{
    m_token = m_scanner.scan();
    return m_token;
}

std::string TranslatorA11::getName()
{
    if(!isAlpha(m_token[0])) expected("name");
    std::string name = m_token;
    nextToken();
    return name;
}

std::string TranslatorA11::getNumber()
{
    std::string number = m_token;
    if(!isDigit(number[0]))
        expected("number");
    bool decimalp = false;
    for(unsigned int i = 1; i < number.length(); i++)
        if(!isDigit(number[i]))
        {
            if(!decimalp && number[i] == '.') decimalp = true;
            else if(i == number.length() - 1)
            {
                if(decimalp && (number[i] == 'f' || number[i] == 'F')) continue;
                else if(!decimalp && (number[i] == 'l' || number[i] == 'L')) continue;
                else expected("number");
            }
            else expected("number");
        }
    nextToken();
    return number;
}

TranslatorA11::Type TranslatorA11::getNumberType(std::string number)
{
    if(number[number.length() - 1] == 'f' || number[number.length() - 1] == 'F')
        return FLOAT;
    for(unsigned int i = 1; i < number.length(); i++)
        if(number[i] == '.') return FLOAT;
    if(number[number.length() - 1] == 'l' || number[number.length() - 1] == 'L')
        return LONG;
    return INT;
}

TranslatorA11::Type TranslatorA11::getType(bool voidAllowed)
{
    std::string strtype = m_token;
    Type type;
    if(strtype == "int") type = INT;
    else if(strtype == "float") type = FLOAT;
    else if(strtype == "long") type = LONG;
    else if(strtype == "double") type = DOUBLE;
    else if(voidAllowed && strtype == "void") type = VOID;
    else expected("type");
    nextToken();
    return type;
}

void TranslatorA11::match(std::string x)
{
    if(m_token != x) expected("'" + x + "'");
    nextToken();
}
