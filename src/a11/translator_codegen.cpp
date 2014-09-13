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

void TranslatorA11::callFunction(std::string name, bool nonVoidOnly)
{
    funmap_it it = m_functions.find(name);
    if(it == m_functions.end())
        abort("function \"" + name + "\" not found near line " + toString(m_scanner.getLine()));

    FunctionData cfun = it->second;

    if(nonVoidOnly && cfun.isVoid)
        abort("void function \"" + name + "\" called in expression near line " + toString(m_scanner.getLine()));

    match("(");

    for(int i = 0; i < cfun.argc; i++)
    {
        if(i) match(",");
        expression();
    }

    match(")");
    writeln("call " + name + " " + toString(cfun.argc));
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

void TranslatorA11::convert(Type type)
{
    switch(type)
    {
    case INT: writeln("cti"); break;
    case FLOAT: writeln("ctf"); break;
    case LONG: writeln("ctl"); break;
    case DOUBLE: writeln("ctd"); break;
    }
}

void TranslatorA11::assignment(std::string name, bool inDeclaration)
{
    match("=");
    expression();
    std::vector<std::string>::iterator localvar = std::find(m_localvars.begin(), m_localvars.end(), name);
    if(localvar != m_localvars.end() || inDeclaration)
    {
        convert(m_lvartype[name]);
        writeln("load " + name);
    }
    else
    {
        std::vector<std::string>::iterator globalvar = std::find(m_globalvars.begin(), m_globalvars.end(), name);
        if(globalvar != m_globalvars.end())
        {
            convert(m_gvartype[name]);
            writeln("loadwide " + name);
        }
        else
            abort("var \"" + name + "\" not declared near line " + toString(m_scanner.getLine()));
    }
}
