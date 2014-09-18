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

TranslatorA11::Type TranslatorA11::exprSuff()
{
    if(isDigit(m_token[0]))
    {
        std::string number = getNumber();
        Type type = getNumberType(number);
        switch(type)
        {
        case INT: writeln("pushi " + number); break;
        case FLOAT: writeln("pushf " + number); break;
        case LONG: writeln("pushl " + number); break;
        case DOUBLE: writeln("pushd " + number); break;
        default: abort("invalid number type near line " + toString(m_scanner.getLine()));
        }
        return type;
    }
    else if(isAlpha(m_token[0]))
    {
        std::string name = getName();
        if(m_token == "(")
        {
            callFunction(name, true);
            return returnType(name);
        }
        /*
        else if(m_token == "[")
        {
            match("[");
            match("]");
        }
        */
        else
        {
            fetchVariable(name);
            return getVariableType(name);
        }
    }
    else if(m_token == "(")
    {
        match("(");
        Type type = expression();
        match(")");
        return type;
    }
    else
    {
        expected("literal or identifier");
    }
    return VOID;
}
TranslatorA11::Type TranslatorA11::exprPref()
{
    Type type;
    if(m_token == OPERATOR_UNARY_PLUS)
    {
        match(OPERATOR_UNARY_PLUS);
        type = exprSuff();
    }
    else if(m_token == OPERATOR_UNARY_MINUS)
    {
        match(OPERATOR_UNARY_MINUS);
        type = exprSuff();
        writeln(INSTRUCTION_UNARY_MINUS);
    }
    else if(m_token == OPERATOR_LOGICAL_NOT)
    {
        match(OPERATOR_LOGICAL_NOT);
        type = exprSuff();
        writeln(INSTRUCTION_LOGICAL_NOT);
    }
    else if(m_token == OPERATOR_BITWISE_NOT)
    {
        match(OPERATOR_BITWISE_NOT);
        type = exprSuff();
        writeln(INSTRUCTION_BITWISE_NOT);
    }
    else
    {
        type = exprSuff();
    }
    return type;
}
/*************************************************/
TranslatorA11::Type TranslatorA11::exprMul()
{
    Type type = exprPref();
    while(m_token == OPERATOR_MULTIPLICATION
       || m_token == OPERATOR_DIVISION
       || m_token == OPERATOR_REMAINDER)
    {
        if(m_token == OPERATOR_MULTIPLICATION)
        {
            match(OPERATOR_MULTIPLICATION);
            type = stackConvert(exprPref(), type);
            writeln(INSTRUCTION_MULTIPLICATION);
        }
        else if(m_token == OPERATOR_DIVISION)
        {
            match(OPERATOR_DIVISION);
            type = stackConvert(exprPref(), type);
            writeln(INSTRUCTION_DIVISION);
        }
        else if(m_token == OPERATOR_REMAINDER)
        {
            match(OPERATOR_REMAINDER);
            type = stackConvert(exprPref(), type);
            writeln(INSTRUCTION_REMAINDER);
        }
    }
    return type;
}
TranslatorA11::Type TranslatorA11::exprAdd()
{
    Type type = exprMul();
    while(m_token == OPERATOR_ADDITION
       || m_token == OPERATOR_SUBTRACTION)
    {
        if(m_token == OPERATOR_ADDITION)
        {
            match(OPERATOR_ADDITION);
            type = stackConvert(exprMul(), type);
            writeln(INSTRUCTION_ADDITION);
        }
        else if(m_token == OPERATOR_SUBTRACTION)
        {
            match(OPERATOR_SUBTRACTION);
            type = stackConvert(exprMul(), type);
            writeln(INSTRUCTION_SUBTRACTION);
        }
    }
    return type;
}
/*************************************************/
TranslatorA11::Type TranslatorA11::exprBShift()
{
    Type type = exprAdd();
    while(m_token == OPERATOR_SHIFTLEFT
       || m_token == OPERATOR_SHIFTRIGHT)
    {
        if(m_token == OPERATOR_SHIFTLEFT)
        {
            match(OPERATOR_SHIFTLEFT);
            type = stackConvert(exprAdd(), type);
            writeln(INSTRUCTION_SHIFTLEFT);
        }
        else if(m_token == OPERATOR_SHIFTRIGHT)
        {
            match(OPERATOR_SHIFTRIGHT);
            type = stackConvert(exprAdd(), type);
            writeln(INSTRUCTION_SHIFTRIGHT);
        }
    }
    return type;
}
/*************************************************/
TranslatorA11::Type TranslatorA11::exprRel()
{
    Type type = exprBShift();
    while(m_token == OPERATOR_LESS
       || m_token == OPERATOR_LESSEQUAL
       || m_token == OPERATOR_GREATER
       || m_token == OPERATOR_GREATEREQUAL)
    {
        if(m_token == OPERATOR_LESS)
        {
            match(OPERATOR_LESS);
            type = stackConvert(exprBShift(), type);
            writeln(INSTRUCTION_LESS);
        }
        else if(m_token == OPERATOR_LESSEQUAL)
        {
            match(OPERATOR_LESSEQUAL);
            type = stackConvert(exprBShift(), type);
            writeln(INSTRUCTION_LESSEQUAL);
        }
        else if(m_token == OPERATOR_GREATER)
        {
            match(OPERATOR_GREATER);
            type = stackConvert(exprBShift(), type);
            writeln(INSTRUCTION_GREATER);
        }
        else if(m_token == OPERATOR_GREATEREQUAL)
        {
            match(OPERATOR_GREATEREQUAL);
            type = stackConvert(exprBShift(), type);
            writeln(INSTRUCTION_GREATEREQUAL);
        }
    }
    return type;
}
TranslatorA11::Type TranslatorA11::exprRelEqual()
{
    Type type = exprRel();
    while(m_token == OPERATOR_EQUAL
       || m_token == OPERATOR_NOT_EQUAL)
    {
        if(m_token == OPERATOR_EQUAL)
        {
            match(OPERATOR_EQUAL);
            type = stackConvert(exprRel(), type);
            writeln(INSTRUCTION_EQUAL);
        }
        else if(m_token == OPERATOR_NOT_EQUAL)
        {
            match(OPERATOR_NOT_EQUAL);
            type = stackConvert(exprRel(), type);
            writeln(INSTRUCTION_NOT_EQUAL);
        }
    }
    return type;
}
/*************************************************/
TranslatorA11::Type TranslatorA11::exprBAND()
{
    Type type = exprRelEqual();
    while(m_token == OPERATOR_BITWISE_AND)
    {
        match(OPERATOR_BITWISE_AND);
        type = stackConvert(exprRelEqual(), type);
        writeln(INSTRUCTION_BITWISE_AND);
    }
    return type;
}
TranslatorA11::Type TranslatorA11::exprBXOR()
{
    Type type = exprBAND();
    while(m_token == OPERATOR_BITWISE_XOR)
    {
        match(OPERATOR_BITWISE_XOR);
        type = stackConvert(exprBAND(), type);
        writeln(INSTRUCTION_BITWISE_XOR);
    }
    return type;
}
TranslatorA11::Type TranslatorA11::exprBOR()
{
    Type type = exprBXOR();
    while(m_token == OPERATOR_BITWISE_OR)
    {
        match(OPERATOR_BITWISE_OR);
        type = stackConvert(exprBXOR(), type);
        writeln(INSTRUCTION_BITWISE_OR);
    }
    return type;
}
/*************************************************/
TranslatorA11::Type TranslatorA11::exprLAND()
{
    Type type = exprBOR();
    while(m_token == OPERATOR_LOGICAL_AND)
    {
        match(OPERATOR_LOGICAL_AND);
        type = stackConvert(exprBOR(), type);
        writeln(INSTRUCTION_LOGICAL_AND);
    }
    return type;
}
TranslatorA11::Type TranslatorA11::exprLOR()
{
    Type type = exprLAND();
    while(m_token == OPERATOR_LOGICAL_OR)
    {
        match(OPERATOR_LOGICAL_OR);
        type = stackConvert(exprLAND(), type);
        writeln(INSTRUCTION_LOGICAL_OR);
    }
    return type;
}
/*************************************************/
TranslatorA11::Type TranslatorA11::expression()
{
    return exprLOR();
}
