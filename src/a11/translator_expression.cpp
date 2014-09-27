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

TranslatorA11::Type TranslatorA11::exprSuff()
{
    if(isDigit(m_token[0]))
    {
        std::string number = getNumber();
        Type type = getNumberType(number);
        instrPush(number, type);
        return type;
    }
    else if(m_token == "new")
    {
        donew();
        return LONG;
    }
    else if(isAlpha(m_token[0]))
    {
        std::string name = getName();
        if(m_token == "(")
        {
            callFunction(name, true);
            return returnType(name);
        }
        else if(m_token == "[")
        {
            return doindex(name);
        }
        else
        {
            fetchVariable(name);
            return getVariableType(name);
        }
    }
    else if(m_token == "@")
    {
        match("@");
        std::string name = getName();
        if(!variableExists(name))
            abortnl("variable '" + name + "' not declared");
        instrPush("@" + name, LONG);
        return LONG;
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
    if(m_token == "-")
    {
        match("-");
        type = exprSuff();
        instrNeg(type);
    }
    else if(m_token == "!")
    {
        match("!");
        type = exprSuff();
        checkInteger(type, "!");
        instrLNOT(type);
    }
    else if(m_token == "~")
    {
        match("~");
        type = exprSuff();
        checkInteger(type, "~");
        instrBNOT(type);
    }
    else
    {
        if(m_token == "+")
            match(m_token);
        type = exprSuff();
    }
    return type;
}
TranslatorA11::Type TranslatorA11::exprCast()
{
    Type ctype = VOID;
    if(m_token == "int") { match("int"); ctype = INT; }
    if(m_token == "float") { match("float"); ctype = FLOAT; }
    if(m_token == "long") { match("long"); ctype = LONG; }
    if(m_token == "double") { match("double"); ctype = DOUBLE; }

    Type type = exprPref();
    if(ctype != VOID)
    {
        if(type == ctype)
            warningnl("unnecessary " + getTypeName(ctype) + " cast");
        else
        {
            convert(type, ctype);
        }
        return ctype;
    }
    return type;
}
TranslatorA11::Type TranslatorA11::exprExtr()
{
    Type ctype = VOID;
    if(m_token == "<-")
    {
        match("<-");
        if(m_token == "int") { match("int"); ctype = INT; }
        else if(m_token == "float") { match("float"); ctype = FLOAT; }
        else if(m_token == "long") { match("long"); ctype = LONG; }
        else if(m_token == "double") { match("double"); ctype = DOUBLE; }
        else expected("type");
    }

    Type type;
    if(ctype == VOID) type = exprCast();
    else
    {
        type = exprExtr();
        if(type != LONG)
            abortnl("invalid operand of type '" + getTypeName(type) + "' "
                    "to operator '<-" + getTypeName(ctype) + "'");
        instrExtr(ctype);
        return ctype;
    }
    return type;
}
/*************************************************/
TranslatorA11::Type TranslatorA11::exprMul()
{
    Type type = exprExtr();
    while(m_token == "*"
       || m_token == "/"
       || m_token == "%")
    {
        if(m_token == "*")
        {
            match("*");
            type = stackConvert(exprExtr(), type);
            instrMul(type);
        }
        else if(m_token == "/")
        {
            match("/");
            type = stackConvert(exprExtr(), type);
            instrDiv(type);
        }
        else if(m_token == "%")
        {
            match("%");
            Type type2 = exprExtr();
            checkInteger2(type, type2, "%");
            type = stackConvert(type2, type);
            instrRem(type);
        }
    }
    return type;
}
TranslatorA11::Type TranslatorA11::exprAdd()
{
    Type type = exprMul();
    while(m_token == "+"
       || m_token == "-")
    {
        if(m_token == "+")
        {
            match("+");
            type = stackConvert(exprMul(), type);
            instrAdd(type);
        }
        else if(m_token == "-")
        {
            match("-");
            type = stackConvert(exprMul(), type);
            instrSub(type);
        }
    }
    return type;
}
/*************************************************/
TranslatorA11::Type TranslatorA11::exprBShift()
{
    Type type = exprAdd();
    while(m_token == "<<"
       || m_token == ">>")
    {
        if(m_token == "<<")
        {
            match("<<");
            Type type2 = exprAdd();
            checkInteger2(type, type2, "<<");
            if(type2 == LONG)
            {
                warningnl("deprecated second operand of type 'long' to operator '<<'");
                convert(LONG, INT);
            }
            instrShl(type);
        }
        else if(m_token == ">>")
        {
            match(">>");
            Type type2 = exprAdd();
            checkInteger2(type, type2, ">>");
            if(type2 == LONG)
            {
                warningnl("deprecated second of operand type 'long' to operator '>>'");
                convert(LONG, INT);
            }
            instrShr(type);
        }
    }
    return type;
}
/*************************************************/
TranslatorA11::Type TranslatorA11::exprRel()
{
    Type type = exprBShift();
    while(m_token == "<"
       || m_token == "<="
       || m_token == ">"
       || m_token == ">=")
    {
        if(m_token == "<")
        {
            match("<");
            Type type2 = exprBShift();
            instrLt(type2, type);
            type = INT;
        }
        else if(m_token == "<=")
        {
            match("<=");
            Type type2 = exprBShift();
            instrLe(type2, type);
            type = INT;
        }
        else if(m_token == ">")
        {
            match(">");
            Type type2 = exprBShift();
            instrGt(type2, type);
            type = INT;
        }
        else if(m_token == ">=")
        {
            match(">=");
            Type type2 = exprBShift();
            instrGe(type2, type);
            type = INT;
        }
    }
    return type;
}
TranslatorA11::Type TranslatorA11::exprRelEqual()
{
    Type type = exprRel();
    while(m_token == "=="
       || m_token == "!=")
    {
        if(m_token == "==")
        {
            match("==");
            Type type2 = exprRel();
            instrEq(type2, type);
            type = INT;
        }
        else if(m_token == "!=")
        {
            match("!=");
            Type type2 = exprRel();
            instrNe(type2, type);
            type = INT;
        }
    }
    return type;
}
/*************************************************/
TranslatorA11::Type TranslatorA11::exprBAND()
{
    Type type = exprRelEqual();
    while(m_token == "&")
    {
        match("&");
        Type type2 = exprRelEqual();
        checkInteger2(type, type2, "&");
        type = stackConvert(type2, type);
        instrBAND(type);
    }
    return type;
}
TranslatorA11::Type TranslatorA11::exprBXOR()
{
    Type type = exprBAND();
    while(m_token == "^")
    {
        match("^");
        Type type2 = exprBAND();
        checkInteger2(type, type2, "^");
        type = stackConvert(type2, type);
        instrBXOR(type);
    }
    return type;
}
TranslatorA11::Type TranslatorA11::exprBOR()
{
    Type type = exprBXOR();
    while(m_token == "|")
    {
        match("|");
        Type type2 = exprBXOR();
        checkInteger2(type, type2, "|");
        type = stackConvert(type2, type);
        instrBOR(type);
    }
    return type;
}
/*************************************************/
TranslatorA11::Type TranslatorA11::exprLAND()
{
    Type type = exprBOR();
    while(m_token == "&&")
    {
        match("&&");
        Type type2 = exprBOR();
        checkInteger2(type, type2, "&&");
        type = stackConvert(type2, type);
        instrLAND(type);
    }
    return type;
}
TranslatorA11::Type TranslatorA11::exprLOR()
{
    Type type = exprLAND();
    while(m_token == "||")
    {
        match("||");
        Type type2 = exprLAND();
        checkInteger2(type, type2, "||");
        type = stackConvert(type2, type);
        instrLOR(type);
    }
    return type;
}
/*************************************************/
TranslatorA11::Type TranslatorA11::expression()
{
    return exprLOR();
}
