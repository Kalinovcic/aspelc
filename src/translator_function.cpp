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

void AspelTranslator::checkFunction(std::string funName)
{
    funmap_it it = m_functions.find(funName);
    if(it == m_functions.end()) return;

    FunctionData previous = (*it).second;

    if(m_cfun.forward) abort("function \"" + funName + "\" reforwarding near line " + toString(m_scanner.getLine()));
    if(!previous.forward) abort("function \"" + funName + "\" redeclaration near line " + toString(m_scanner.getLine()));

    if(m_cfun.isVoid != previous.isVoid) abort("function \"" + funName + "\" inconsistency near line " + toString(m_scanner.getLine()));
    if(m_cfun.argc != previous.argc) abort("function \"" + funName + "\" inconsistency near line " + toString(m_scanner.getLine()));
}

void AspelTranslator::function()
{
    match("function");

    bool native = false;
    if(m_token == "native")
    {
        match("native");
        native = true;
    }

    m_cfun.isVoid = false;
    if(m_token == "void")
    {
        match("void");
        m_cfun.isVoid = true;
    }

    std::string functionName = m_token;
    std::vector<std::string> argvars;
    nextToken();

    match("(");
    if(m_token != ")")
    {
        argvars.push_back(getName());
        while(m_token == ",")
        {
            match(",");
            argvars.push_back(getName());
        }
    }
    m_cfun.argc = argvars.size();
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
        }

        block("-", "-");
        write("\t.\n");
        m_localvars.clear();
    }
    else
    {
        if(native)
        {
            write("f:n");
            if(m_cfun.isVoid)
                write("v");
            write("\t" + functionName + "\n\n");
        }
        match(";");
    }
}
