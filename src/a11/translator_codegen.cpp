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
    write(labelname + ":");
}

TranslatorA11::Type TranslatorA11::returnType(std::string name)
{
    funmap_it it = m_functions.find(name);
    if(it == m_functions.end())
        abortnl("function \"" + name + "\" not found");
    return it->second.rtype;
}

void TranslatorA11::callFunction(std::string name, bool nonVoidOnly)
{
    funmap_it it = m_functions.find(name);
    if(it == m_functions.end())
        abortnl("function \"" + name + "\" not found");

    FunctionData cfun = it->second;

    if(nonVoidOnly && cfun.rtype == VOID)
        abortnl("void function \"" + name + "\" called in expression");

    match("(");

    for(unsigned int i = 0; i < cfun.atype.size(); i++)
    {
        if(i) match(",");
        Type type = expression();
        if(type != cfun.atype[i])
            convert(type, cfun.atype[i]);
    }

    match(")");
    writeln("call " + name);
}

bool TranslatorA11::variableExists(std::string name)
{
    std::map<std::string, Type>::iterator localvar = m_lvartype.find(name);
    if(localvar != m_lvartype.end())
        return true;
    else
    {
        std::map<std::string, Type>::iterator globalvar = m_gvartype.find(name);
        if(globalvar != m_gvartype.end())
            return true;
    }
    return false;
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
    abortnl("var \"" + name + "\" not declared");
    return VOID;
}

void TranslatorA11::fetchVariable(std::string name)
{
    std::vector<std::string>::iterator localvar = std::find(m_localvars.begin(), m_localvars.end(), name);
    if(localvar != m_localvars.end())
        instrFetch(name, m_lvartype[name]);
    else
    {
        std::vector<std::string>::iterator globalvar = std::find(m_globalvars.begin(), m_globalvars.end(), name);
        if(globalvar != m_globalvars.end())
            instrFetchWide(name, m_gvartype[name]);
        else
            abortnl("var \"" + name + "\" not declared");
    }
}

void TranslatorA11::swap(Type top, Type next)
{
    if(top == VOID || next == VOID)
        abortnl("invalid use of void type");
    if(top == LONG || top == DOUBLE)
    {
        if(next == LONG || next == DOUBLE) writeln("swap8");
        else if(next == BYTE || next == SHORT || next == INT || next == FLOAT) writeln("swap84");
        else abort("invalid swap types");
    }
    else if(top == BYTE || top == SHORT || top == INT || top == FLOAT)
    {
        if(next == LONG || next == DOUBLE) writeln("swap48");
        else if(next == BYTE || next == SHORT || next == INT || next == FLOAT) writeln("swap4");
        else abort("invalid swap types");
    }
    else abort("invalid swap types");
}

void TranslatorA11::convert(Type from, Type to)
{
    if(from == BYTE || from == SHORT)
    {
        if(to != INT) convert(INT, to);
        return;
    }
    if(to == BYTE || to == SHORT)
    {
        if(from != INT) convert(from, INT);
        if(to == BYTE) writeln("ci41");
        else if(to == SHORT) writeln("ci42");
        return;
    }

    switch(from)
    {
    case INT:
        switch(to)
        {
        case INT: abortnl("invalid conversion - int to int"); break;
        case FLOAT: writeln("cif4"); break;
        case LONG: writeln("cili"); break;
        case DOUBLE: writeln("cili"); writeln("cif8"); break;
        default: expected("non-void type");
        }
        break;
    case FLOAT:
        switch(to)
        {
        case INT: writeln("cfi4"); break;
        case FLOAT: abortnl("invalid conversion - float to float"); break;
        case LONG: writeln("cf48"); writeln("cfi8"); break;
        case DOUBLE: writeln("cf48"); break;
        default: expected("non-void type");
        }
        break;
    case LONG:
        switch(to)
        {
        case INT: writeln("ci84"); break;
        case FLOAT: writeln("ci84"); writeln("cif4"); break;
        case LONG: abortnl("invalid conversion - long to long"); break;
        case DOUBLE: writeln("cif8"); break;
        default: expected("non-void type");
        }
        break;
    case DOUBLE:
        switch(to)
        {
        case INT: writeln("cf84"); writeln("cfi4"); break;
        case FLOAT: writeln("cf84"); break;
        case LONG: writeln("cfi8"); break;
        case DOUBLE: abortnl("invalid conversion - double to double"); break;
        default: expected("non-void type");
        }
        break;
    default: expected("non-void type");
    }
}

std::string TranslatorA11::getTypeName(Type type)
{
    switch(type)
    {
    case BYTE: return "byte";
    case SHORT: return "short";
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
        warningnl("converting " + getTypeName(original) + " to " + getTypeName(converted) + " without a cast");
}

TranslatorA11::Type TranslatorA11::greaterType(Type type1, Type type2)
{
    if(type1 == VOID || type2 == VOID)
        abortnl("invalid use of void type");
    if(type1 == DOUBLE || type2 == DOUBLE) return DOUBLE;
    if(type1 == FLOAT || type2 == FLOAT) return FLOAT;
    if(type1 == LONG || type2 == LONG) return LONG;
    if(type1 == BYTE || type1 == SHORT || type1 == INT
    || type2 == BYTE || type2 == SHORT || type2 == INT) return INT;
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
            convert(next, top);
            swap(top, top);
        }
    }
    else
    {
        conversionWarning(top, next);
        convert(top, next);
    }
    return greater;
}

void TranslatorA11::normalAssignment(std::string name, bool inDeclaration)
{
    match("=");
    Type type = expression();
    Type vartype = getVariableType(name);
    if(type != vartype)
    {
        conversionWarning(type, vartype);
        convert(type, vartype);
    }

    std::vector<std::string>::iterator localvar = std::find(m_localvars.begin(), m_localvars.end(), name);
    if(localvar != m_localvars.end() || inDeclaration)
        instrLoad(name, m_lvartype[name]);
    else
    {
        std::vector<std::string>::iterator globalvar = std::find(m_globalvars.begin(), m_globalvars.end(), name);
        if(globalvar != m_globalvars.end())
            instrLoadWide(name, m_gvartype[name]);
        else
            abortnl("var \"" + name + "\" not declared");
    }
}
void TranslatorA11::valueAssignment()
{
    match("->");
    Type type = expression();
    instrRefLoad(type);
}

void TranslatorA11::assignment(std::string name, bool inDeclaration)
{
    if(m_token == "=")
        normalAssignment(name, inDeclaration);
    else if(m_token == "->")
    {
        Type vartype = getVariableType(name);
        if(vartype != LONG)
            abortnl("using type '" + getTypeName(vartype) + "' as reference");
        fetchVariable(name);
        valueAssignment();
    }
    else expected("= or ->");
}

void TranslatorA11::assignmentComplex()
{
    match("(");
    Type exprtype = expression();
    if(exprtype != LONG)
        abortnl("invalid expression of type '" + getTypeName(exprtype) + "' used in expression assignment");
    match(")");
    if(m_token == "=")
        abortnl("assignment operator '=' used in expression assignment");
    valueAssignment();
}

void TranslatorA11::donew()
{
    match("new");
    match("[");
    std::string size = getNumber();
    Type numtype = getNumberType(size);
    if(numtype != INT && numtype != LONG)
        abortnl("invalid operand of type '" + getTypeName(numtype) + "' to operator 'new'");
    match("]");

    writeln("alloc " + size);
}

void TranslatorA11::dodelete()
{
    match("delete");
    std::string name = getName();
    fetchVariable(name);
    writeln("free");
}

void TranslatorA11::dosize()
{
    match("size");
    match("(");
    Type type = getType(false);
    switch(type)
    {
    case BYTE: instrPush("1", LONG); break;
    case SHORT: instrPush("2", LONG); break;
    case INT: instrPush("4", LONG); break;
    case FLOAT: instrPush("4", LONG); break;
    case LONG: instrPush("8", LONG); break;
    case DOUBLE: instrPush("8", LONG); break;
    default: abortnl("invalid use of types");
    }
    match(")");
}

