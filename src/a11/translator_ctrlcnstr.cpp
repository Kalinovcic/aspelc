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
 * File: translator_ctrlcnstr.cpp
 * Description: Methods from AspelTranslator for translating control constructs.
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

void TranslatorA11::condition()
{
    match("(");
    if(m_token == ")")
        expected("condition");
    expression();
    match(")");
}

void TranslatorA11::doif(std::string breakLabel, std::string continueLabel)
{
    match("if");
    condition();

    std::string toElse = newLabel();
    std::string toEnd;

    writeln("ifn " + toElse);

    block(breakLabel, continueLabel);

    bool hasElse = m_token == "else";

    if(hasElse)
    {
        match("else");

        toEnd = newLabel();
        writeln("goto " + toEnd);
    }

    writeLabel(toElse);

    if(hasElse)
    {
        block(breakLabel, continueLabel);
        writeLabel(toEnd);
    }
}

void TranslatorA11::dowhile()
{
    match("while");

    std::string toStart = newLabel();
    std::string toEnd = newLabel();

    writeLabel(toStart);
    condition();
    writeln("ifn " + toEnd);
    block(toEnd, toStart);
    writeln("goto " + toStart);
    writeLabel(toEnd);
}

void TranslatorA11::dobreak(std::string breakLabel)
{
    match("break");
    if(breakLabel == "-")
        abort("no label to break to");
    writeln("goto " + breakLabel);
}

void TranslatorA11::docontinue(std::string continueLabel)
{
    match("continue");
    if(continueLabel == "-")
        abort("no label to continue from");
    writeln("goto " + continueLabel);
}

void TranslatorA11::doreturn()
{
    match("return");
    if(!m_cfun.isVoid)
        expression();
    writeln("return");
}
