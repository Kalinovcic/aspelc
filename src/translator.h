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
 * File: translator.h
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

#include "scanner.h"

class AspelTranslator
{
public:
    AspelTranslator(LexicalScanner& scanner, std::ostream& out);
    ~AspelTranslator();

    inline void testf() { block("-", "-"); }
private:
    std::string m_token;
    int m_labelCounter;

    LexicalScanner& m_scanner;
    std::ostream& m_out;

    // base
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlnum(char c) const;
    bool isAddop(std::string c) const;
    bool isMulop(std::string c) const;
    bool isOrop(std::string c) const;
    bool isRelop(std::string c) const;
    bool isBool(std::string c) const;
    bool isKeyword(std::string c) const;

    // error
    void error(std::string message) const;
    void abort(std::string reason) const;
    void expected(std::string item) const;

    // input
    std::string nextToken();
    std::string getName();
    std::string getNumber();
    void match(std::string x);

    // output
    void write(std::string cont);
    void writeln(std::string cont);

    // codegen
    std::string newLabel();
    void writeLabel(std::string labelName);
    void callFunction(std::string name);

    // expression
    void exprSuff();
    void exprPref();
    void exprMul();
    void exprAdd();
    void exprBShift();
    void exprRel();
    void exprRelEqual();
    void exprBAND();
    void exprBXOR();
    void exprBOR();
    void exprLAND();
    void exprLOR();
    void expression();

    inline void assignment(std::string name)
    {
        match("=");
        expression();
        writeln("load " + name);
    }

    inline void block(std::string breakLabel, std::string continueLabel)
    {
        match("{");
        while(m_token != "}")
        {
            if(isKeyword(m_token))
            {
                if(m_token == "if") doif(breakLabel, continueLabel);
                else if(m_token == "while") dowhile();
                else
                {
                    if(m_token == "break") dobreak(breakLabel);
                    else if(m_token == "continue") docontinue(breakLabel);
                    match(";");
                }
            }
            else
            {
                std::string name = getName();
                if(m_token == "=") assignment(name);
                else if(m_token == "(") callFunction(name);
                else expected("statement");
                match(";");
            }
        }
        match("}");
    }

    inline void condition()
    {
        match("(");
        if(m_token == ")")
            expected("condition");
        expression();
        match(")");
    }

    inline void doif(std::string breakLabel, std::string continueLabel)
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

    inline void dowhile()
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

    inline void dobreak(std::string breakLabel)
    {
        match("break");
        if(breakLabel == "-")
            abort("no label to break to");
        writeln("goto " + breakLabel);
    }

    inline void docontinue(std::string continueLabel)
    {
        match("continue");
        if(continueLabel == "-")
            abort("no label to continue from");
        writeln("goto " + continueLabel);
    }

    inline std::string toString(int val)   const { std::stringstream ss; ss << val; return ss.str(); }
};

#endif /* TRANSLATOR_H_ */
