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
 * Description: Methods from AspelTranslator for translating expressions.
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

#define OPERATOR_UNARY_PLUS     "+"
#define OPERATOR_UNARY_MINUS    "-"
#define OPERATOR_LOGICAL_NOT    "!"
#define OPERATOR_BITWISE_NOT    "~"
#define OPERATOR_MULTIPLICATION "*"
#define OPERATOR_DIVISION       "/"
#define OPERATOR_REMAINDER      "%"
#define OPERATOR_ADDITION       "+"
#define OPERATOR_SUBTRACTION    "-"
#define OPERATOR_SHIFTLEFT      "<<"
#define OPERATOR_SHIFTRIGHT     ">>"
#define OPERATOR_LESS           "<"
#define OPERATOR_LESSEQUAL      "<="
#define OPERATOR_GREATER        ">"
#define OPERATOR_GREATEREQUAL   ">="
#define OPERATOR_EQUAL          "=="
#define OPERATOR_NOT_EQUAL      "!="
#define OPERATOR_BITWISE_AND    "&"
#define OPERATOR_BITWISE_XOR    "^"
#define OPERATOR_BITWISE_OR     "|"
#define OPERATOR_LOGICAL_AND    "&&"
#define OPERATOR_LOGICAL_OR     "||"

#define INSTRUCTION_UNARY_PLUS     ""
#define INSTRUCTION_UNARY_MINUS    "neg"
#define INSTRUCTION_LOGICAL_NOT    "lnot"
#define INSTRUCTION_BITWISE_NOT    "not"
#define INSTRUCTION_MULTIPLICATION "mul"
#define INSTRUCTION_DIVISION       "div"
#define INSTRUCTION_REMAINDER      "rem"
#define INSTRUCTION_ADDITION       "add"
#define INSTRUCTION_SUBTRACTION    "sub"
#define INSTRUCTION_SHIFTLEFT      "shr"
#define INSTRUCTION_SHIFTRIGHT     "shl"
#define INSTRUCTION_LESS           "lt"
#define INSTRUCTION_LESSEQUAL      "le"
#define INSTRUCTION_GREATER        "gt"
#define INSTRUCTION_GREATEREQUAL   "ge"
#define INSTRUCTION_EQUAL          "eq"
#define INSTRUCTION_NOT_EQUAL      "ne"
#define INSTRUCTION_BITWISE_AND    "and"
#define INSTRUCTION_BITWISE_XOR    "xor"
#define INSTRUCTION_BITWISE_OR     "or"
#define INSTRUCTION_LOGICAL_AND    "land"
#define INSTRUCTION_LOGICAL_OR     "lor"

void AspelTranslator::exprSuff()
{
    if(isDigit(m_token[0]))
    {
        std::string number = getNumber();
        writeln("push " + number);
    }
    else if(isAlpha(m_token[0]))
    {
        std::string name = getName();
        if(m_token == "(")
        {
            callFunction(name, true);
        }
        else if(m_token == "[")
        {
            match("[");
            // get index
            match("]");
            writeln("<array get>");
        }
        else
        {
            if(isBool(name)) writeln("push " + name);
            else fetchVariable(name);
        }
    }
    else if(m_token == "(")
    {
        match("(");
        expression();
        match(")");
    }
    else
    {
        expected("literal or identifier");
    }
}
void AspelTranslator::exprPref()
{
    if(m_token == OPERATOR_UNARY_PLUS)
    {
        match(OPERATOR_UNARY_PLUS);
        exprSuff();
    }
    else if(m_token == OPERATOR_UNARY_MINUS)
    {
        match(OPERATOR_UNARY_MINUS);
        exprSuff();
        writeln(INSTRUCTION_UNARY_MINUS);
    }
    else if(m_token == OPERATOR_LOGICAL_NOT)
    {
        match(OPERATOR_LOGICAL_NOT);
        exprSuff();
        writeln(INSTRUCTION_LOGICAL_NOT);
    }
    else if(m_token == OPERATOR_BITWISE_NOT)
    {
        match(OPERATOR_BITWISE_NOT);
        exprSuff();
        writeln(INSTRUCTION_BITWISE_NOT);
    }
    else
    {
        exprSuff();
    }
}
/*************************************************/
void AspelTranslator::exprMul()
{
    exprPref();
    while(m_token == OPERATOR_MULTIPLICATION
       || m_token == OPERATOR_DIVISION)
    {
        if(m_token == OPERATOR_MULTIPLICATION)
        {
            match(OPERATOR_MULTIPLICATION);
            exprPref();
            writeln(INSTRUCTION_MULTIPLICATION);
        }
        else if(m_token == OPERATOR_DIVISION)
        {
            match(OPERATOR_DIVISION);
            exprPref();
            writeln(INSTRUCTION_DIVISION);
        }
    }
}
void AspelTranslator::exprAdd()
{
    exprMul();
    while(m_token == OPERATOR_ADDITION
       || m_token == OPERATOR_SUBTRACTION)
    {
        if(m_token == OPERATOR_ADDITION)
        {
            match(OPERATOR_ADDITION);
            exprMul();
            writeln(INSTRUCTION_ADDITION);
        }
        else if(m_token == OPERATOR_SUBTRACTION)
        {
            match(OPERATOR_SUBTRACTION);
            exprMul();
            writeln(INSTRUCTION_SUBTRACTION);
        }
    }
}
/*************************************************/
void AspelTranslator::exprBShift()
{
    exprAdd();
    while(m_token == OPERATOR_SHIFTLEFT
       || m_token == OPERATOR_SHIFTRIGHT)
    {
        if(m_token == OPERATOR_SHIFTLEFT)
        {
            match(OPERATOR_SHIFTLEFT);
            exprAdd();
            writeln(INSTRUCTION_SHIFTLEFT);
        }
        else if(m_token == OPERATOR_SHIFTRIGHT)
        {
            match(OPERATOR_SHIFTRIGHT);
            exprAdd();
            writeln(INSTRUCTION_SHIFTRIGHT);
        }
    }
}
/*************************************************/
void AspelTranslator::exprRel()
{
    exprBShift();
    while(m_token == OPERATOR_LESS
       || m_token == OPERATOR_LESSEQUAL
       || m_token == OPERATOR_GREATER
       || m_token == OPERATOR_GREATEREQUAL)
    {
        if(m_token == OPERATOR_LESS)
        {
            match(OPERATOR_LESS);
            exprBShift();
            writeln(INSTRUCTION_LESS);
        }
        else if(m_token == OPERATOR_LESSEQUAL)
        {
            match(OPERATOR_LESSEQUAL);
            exprBShift();
            writeln(INSTRUCTION_LESSEQUAL);
        }
        else if(m_token == OPERATOR_GREATER)
        {
            match(OPERATOR_GREATER);
            exprBShift();
            writeln(INSTRUCTION_GREATER);
        }
        else if(m_token == OPERATOR_GREATEREQUAL)
        {
            match(OPERATOR_GREATEREQUAL);
            exprBShift();
            writeln(INSTRUCTION_GREATEREQUAL);
        }
    }
}
void AspelTranslator::exprRelEqual()
{
    exprRel();
    while(m_token == OPERATOR_EQUAL
       || m_token == OPERATOR_NOT_EQUAL)
    {
        if(m_token == OPERATOR_EQUAL)
        {
            match(OPERATOR_EQUAL);
            exprRel();
            writeln(INSTRUCTION_EQUAL);
        }
        else if(m_token == OPERATOR_NOT_EQUAL)
        {
            match(OPERATOR_NOT_EQUAL);
            exprRel();
            writeln(INSTRUCTION_NOT_EQUAL);
        }
    }
}
/*************************************************/
void AspelTranslator::exprBAND()
{
    exprRelEqual();
    while(m_token == OPERATOR_BITWISE_AND)
    {
        match(OPERATOR_BITWISE_AND);
        exprRelEqual();
        writeln(INSTRUCTION_BITWISE_AND);
    }
}
void AspelTranslator::exprBXOR()
{
    exprBAND();
    while(m_token == OPERATOR_BITWISE_XOR)
    {
        match(OPERATOR_BITWISE_XOR);
        exprBAND();
        writeln(INSTRUCTION_BITWISE_XOR);
    }
}
void AspelTranslator::exprBOR()
{
    exprBXOR();
    while(m_token == OPERATOR_BITWISE_OR)
    {
        match(OPERATOR_BITWISE_OR);
        exprBXOR();
        writeln(INSTRUCTION_BITWISE_OR);
    }
}
/*************************************************/
void AspelTranslator::exprLAND()
{
    exprBOR();
    while(m_token == OPERATOR_LOGICAL_AND)
    {
        match(OPERATOR_LOGICAL_AND);
        exprBOR();
        writeln(INSTRUCTION_LOGICAL_AND);
    }
}
void AspelTranslator::exprLOR()
{
    exprLAND();
    while(m_token == OPERATOR_LOGICAL_OR)
    {
        match(OPERATOR_LOGICAL_OR);
        exprLAND();
        writeln(INSTRUCTION_LOGICAL_OR);
    }
}
/*************************************************/
void AspelTranslator::expression()
{
    exprLOR();
}
