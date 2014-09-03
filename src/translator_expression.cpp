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
 * File: translator_expression.cpp
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

void AspelTranslator::identifier()
{
    std::string name = getName();
    if(m_token == "(")
        callFunction(name);
    else
        writeln("fetch " + name);
}

void AspelTranslator::factor()
{
    if(m_token == "(")
    {
        match("(");
        expression();
        match(")");
    }
    else if(isAlpha(m_token[0]))
    {
        identifier();
    }
    else
    {
        writeln("push " + getNumber());
    }
}

void AspelTranslator::signedFactor()
{
    if(m_token == "+") nextToken();
    else if(m_token == "-")
    {
        nextToken();
        if(isDigit(m_token[0]))
        {
            writeln("push -" + getNumber());
        }
        else
        {
            factor();
            writeln("neg");
        }
    }
    else
    {
        factor();
    }
}

void AspelTranslator::mul()
{
    match("*");
    factor();
    writeln("mul");
}

void AspelTranslator::div()
{
    match("/");
    factor();
    writeln("div");
}

void AspelTranslator::term()
{
    signedFactor();
    while(isMulop(m_token))
    {
        if(m_token == "*") mul();
        else if(m_token == "/") div();
    }
}

void AspelTranslator::add()
{
    match("+");
    term();
    writeln("add");
}

void AspelTranslator::sub()
{
    match("-");
    term();
    writeln("sub");
}

void AspelTranslator::expression()
{
    term();
    while(isAddop(m_token))
    {
        if(m_token == "+") add();
        else if(m_token == "-") sub();
    }
}

void AspelTranslator::equals()
{
    match("=");
    expression();
    writeln("eq");
}

void AspelTranslator::notEqual()
{
    match("!=");
    expression();
    writeln("ne");
}

void AspelTranslator::less()
{
    match("<");
    expression();
    writeln("lt");
}

void AspelTranslator::greater()
{
    match(">");
    expression();
    writeln("gt");
}

void AspelTranslator::relation()
{
    expression();
    if(isRelop(m_token))
    {
        if(m_token == "=") equals();
        else if(m_token == "!=") notEqual();
        else if(m_token == "<") less();
        else if(m_token == ">") greater();
    }
}

bool AspelTranslator::getBool()
{
    if(!isBool(m_token)) expected("boolean literal");
    bool result = m_token == "true";
    nextToken();
    return result;
}

void AspelTranslator::boolFactor()
{
    if(isBool(m_token))
    {
        if(getBool()) writeln("push true");
        else writeln("push false");
    }
    else relation();
}

void AspelTranslator::notFactor()
{
    if(m_token == "!")
    {
        match("!");
        boolFactor();
        writeln("not");
    }
    else
    {
        boolFactor();
    }
}

void AspelTranslator::booland()
{
    match("&");
    notFactor();
    writeln("and");
}

void AspelTranslator::boolTerm()
{
    notFactor();
    while(m_token == "&")
        booland();
}

void AspelTranslator::boolor()
{
    match("|");
    boolTerm();
    writeln("or");
}

void AspelTranslator::boolxor()
{
    match("^");
    boolTerm();
    writeln("xor");
}

void AspelTranslator::boolExpression()
{
    boolTerm();

    while(isOrop(m_token))
    {
        if(m_token == "|") boolor();
        else if(m_token == "^") boolxor();
    }
}
