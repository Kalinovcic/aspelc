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
 * File: translator.cpp
 * Description: Basic methods from AspelTranslator for recognizing token types.
 *              Also contains a constructor and destructor.
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

static const std::string addops[]   = { "+", "-", "" };
static const std::string mulops[]   = { "*", "/", "" };
static const std::string orops[]    = { "|", "^", "" };
static const std::string relops[]   = { "==", "!=", "<", "<=", ">", ">=", ""};
static const std::string keywords[] = { "if", "while", "break", "continue", "return", "var", "" };

static inline bool inRange(char c, char begin, char end) { return c >= begin && c <= end; }
static inline bool inList(std::string c, std::string const* list)
{
    std::string current = *list;
    while(current != "")
    {
        if(c == current)
            return true;
        list++;
        current = *list;
    }
    return false;
}

TranslatorA11::TranslatorA11(LexicalScanner& scanner, std::ostream& out)
: Translator(scanner, out)
{
    m_labelCounter = 0;

    nextToken();
}

TranslatorA11::~TranslatorA11()
{

}

bool TranslatorA11::isAlpha(char c)          const { return inRange(c, 'a', 'z') || inRange(c, 'a', 'z'); }
bool TranslatorA11::isDigit(char c)          const { return inRange(c, '0', '9'); }
bool TranslatorA11::isAlnum(char c)          const { return isAlpha(c) || isDigit(c); }
bool TranslatorA11::isAddop(std::string c)   const { return inList(c, addops); }
bool TranslatorA11::isMulop(std::string c)   const { return inList(c, mulops); }
bool TranslatorA11::isOrop(std::string c)    const { return inList(c, orops); }
bool TranslatorA11::isRelop(std::string c)   const { return inList(c, relops); }
bool TranslatorA11::isBool(std::string c)    const { return c == "true" || c == "false"; }
bool TranslatorA11::isKeyword(std::string c) const { return inList(c, keywords); }
