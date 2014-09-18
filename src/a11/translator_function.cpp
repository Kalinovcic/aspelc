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
 * File: translator_function.cpp
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

void TranslatorA11::checkFunction(std::string funName)
{
    funmap_it it = m_functions.find(funName);
    if(it == m_functions.end()) return;

    FunctionData previous = (*it).second;

    if(m_cfun.forward) abort("function \"" + funName + "\" reforwarding near line " + toString(m_scanner.getLine()));
    if(!previous.forward) abort("function \"" + funName + "\" redeclaration near line " + toString(m_scanner.getLine()));

    if(m_cfun.rtype != previous.rtype) abort("function \"" + funName + "\" inconsistency near line " + toString(m_scanner.getLine()));
    if(m_cfun.atype.size() != previous.atype.size())
        abort("function \"" + funName + "\" inconsistency near line " + toString(m_scanner.getLine()));

    for(unsigned int i = 0; i < m_cfun.atype.size(); i++)
        if(m_cfun.atype[i] != previous.atype[i])
            abort("function \"" + funName + "\" inconsistency near line " + toString(m_scanner.getLine()));
}

void TranslatorA11::function()
{
    match("function");

    m_cfun.rtype = getType(true);
    m_cfun.atype.clear();

    bool native = false;
    if(m_token == "native")
    {
        match("native");
        native = true;
    }

    std::string functionName = m_token;
    std::vector<std::string> argvars;
    nextToken();

    match("(");
    if(m_token != ")")
    {
        m_cfun.atype.push_back(getType(false));
        argvars.push_back(getName());
        while(m_token == ",")
        {
            match(",");
            m_cfun.atype.push_back(getType(false));
            argvars.push_back(getName());
        }
    }
    match(")");

    m_cfun.forward = false;
    if(m_token == "forward")
    {
        match("forward");
        if(!native)
            m_cfun.forward = true;
        else
            abort("native function \"" + functionName + "\" forwarded near line " + toString(m_scanner.getLine()));
    }

    checkFunction(functionName);
    m_functions[functionName] = m_cfun;

    if(!m_cfun.forward && !native)
    {
        write("f:\t" + functionName + "\n");
        for(unsigned int i = argvars.size(); i; i--)
        {
            writeln("load " + argvars[i - 1]);
            m_localvars.push_back(argvars[i - 1]);
            m_lvartype[argvars[i - 1]] = m_cfun.atype[i - 1];
        }

        block("-", "-");
        write("\t.\n");
        m_localvars.clear();
        m_lvartype.clear();
    }
    else
    {
        if(native)
        {
            write("n:");
            write("\t" + functionName + "\n\n");
        }
        match(";");
    }
}
