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
 * Description: Methods from AspelTranslator for code generation.
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

#include <algorithm>

std::string TranslatorA11::newLabel()
{
    return "l" + toString(m_labelCounter++);
}

void TranslatorA11::writeLabel(std::string labelname)
{
    write("\n" + labelname + ":");
}

TranslatorA11::Type TranslatorA11::returnType(std::string name)
{
    funmap_it it = m_functions.find(name);
    if(it == m_functions.end())
        abort("function \"" + name + "\" not found near line " + toString(m_scanner.getLine()));
    return it->second.rtype;
}

void TranslatorA11::callFunction(std::string name, bool nonVoidOnly)
{
    funmap_it it = m_functions.find(name);
    if(it == m_functions.end())
        abort("function \"" + name + "\" not found near line " + toString(m_scanner.getLine()));

    FunctionData cfun = it->second;

    if(nonVoidOnly && cfun.rtype == VOID)
        abort("void function \"" + name + "\" called in expression near line " + toString(m_scanner.getLine()));

    match("(");

    for(unsigned int i = 0; i < cfun.atype.size(); i++)
    {
        if(i) match(",");
        expression();
        convert(cfun.atype[i]);
    }

    match(")");
    writeln("call " + name + " " + toString(cfun.atype.size()));
}

TranslatorA11::Type TranslatorA11::getVariableType(std::string name)
{
    std::map<std::string, Type>::iterator localvar = m_lvartype.find(name);
    if(localvar != m_lvartype.end())
        return localvar->second;
    else
    {
        std::map<std::string, Type>::iterator globalvar = m_gvartype.find(name);
        if(globalvar != m_gvartype.end())
            return globalvar->second;
    }
    abort("var \"" + name + "\" not declared near line " + toString(m_scanner.getLine()));
    return VOID;
}

void TranslatorA11::fetchVariable(std::string name)
{
    std::vector<std::string>::iterator localvar = std::find(m_localvars.begin(), m_localvars.end(), name);
    if(localvar != m_localvars.end())
        writeln("fetch " + name);
    else
    {
        std::vector<std::string>::iterator globalvar = std::find(m_globalvars.begin(), m_globalvars.end(), name);
        if(globalvar != m_globalvars.end())
            writeln("fetchwide " + name);
        else
            abort("var \"" + name + "\" not declared near line " + toString(m_scanner.getLine()));
    }
}

void TranslatorA11::swap(Type top, Type next)
{
    if(top == VOID || next == VOID)
        abort("invalid use of void type near line " + toString(m_scanner.getLine()));
    if(top == LONG || top == DOUBLE)
    {
        if(next == LONG || next == DOUBLE) writeln("swap8");
        else if(next == INT || next == FLOAT) writeln("swap84");
        else abort("invalid swap types");
    }
    else if(top == INT || top == FLOAT)
    {
        if(next == LONG || next == DOUBLE) writeln("swap48");
        else if(next == INT || next == FLOAT) writeln("swap4");
        else abort("invalid swap types");
    }
    else abort("invalid swap types");
}

void TranslatorA11::convert(Type type)
{
    switch(type)
    {
    case INT: writeln("cti"); break;
    case FLOAT: writeln("ctf"); break;
    case LONG: writeln("ctl"); break;
    case DOUBLE: writeln("ctd"); break;
    default: expected("non-void type");
    }
}

std::string TranslatorA11::getTypeName(Type type)
{
    switch(type)
    {
    case INT: return "int";
    case FLOAT: return "float";
    case LONG: return "long";
    case DOUBLE: return "double";
    case VOID: return "void";
    }
    return "";
}

void TranslatorA11::conversionWarning(Type original, Type converted)
{
    if(m_wcast)
        warning("converting " + getTypeName(original) + " to " + getTypeName(converted) + " without a cast near line " + toString(m_scanner.getLine()));
}

TranslatorA11::Type TranslatorA11::greaterType(Type type1, Type type2)
{
    if(type1 == VOID || type2 == VOID)
        abort("invalid use of void type near line " + toString(m_scanner.getLine()));
    if(type1 == DOUBLE || type2 == DOUBLE) return DOUBLE;
    if(type1 == FLOAT || type2 == FLOAT) return FLOAT;
    if(type1 == LONG || type2 == LONG) return LONG;
    if(type1 == INT || type2 == INT) return INT;
    abort("invalid type comparison");
    return VOID;
}

TranslatorA11::Type TranslatorA11::stackConvert(Type top, Type next)
{
    Type greater = greaterType(top, next);
    if(top == greater)
    {
        if(next != greater)
        {
            conversionWarning(next, top);
            swap(top, next);
            convert(greater);
            swap(greater, greater);
        }
    }
    else
    {
        conversionWarning(top, next);
        convert(greater);
    }
    return greater;
}

void TranslatorA11::assignment(std::string name, bool inDeclaration)
{
    match("=");
    Type type = expression();
    Type vartype = getVariableType(name);
    if(type != vartype)
    {
        conversionWarning(type, vartype);
        convert(vartype);
    }

    std::vector<std::string>::iterator localvar = std::find(m_localvars.begin(), m_localvars.end(), name);
    if(localvar != m_localvars.end() || inDeclaration)
        writeln("load " + name);
    else
    {
        std::vector<std::string>::iterator globalvar = std::find(m_globalvars.begin(), m_globalvars.end(), name);
        if(globalvar != m_globalvars.end())
            writeln("loadwide " + name);
        else
            abort("var \"" + name + "\" not declared near line " + toString(m_scanner.getLine()));
    }
}
