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
 * Description: Contains the AspelTranslator class, used for translating Aspel
 *              code to AML (Aspel Mnemonic Language).
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

#include <vector>
#include <map>

#include "scanner.h"

struct FunctionData
{
    int argc;
    bool isVoid;
    bool forward;
};

class AspelTranslator
{
public:
    AspelTranslator(LexicalScanner& scanner, std::ostream& out);
    ~AspelTranslator();

    inline void testf() { program(); }
private:
    std::string m_token;
    int m_labelCounter;

    LexicalScanner& m_scanner;
    std::ostream& m_out;

    FunctionData m_cfun;
    std::map<std::string, FunctionData> m_functions;
    std::vector<std::string> m_globalvars;
    std::vector<std::string> m_localvars;

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
    void callFunction(std::string name, bool nonVoidOnly);
    void fetchVariable(std::string name);
    void assignment(std::string name);

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

    void checkFunction(std::string funName);
    void function();

    inline void program()
    {
        while(m_token != "")
        {
            if(m_token == "function") function();
            else if(m_token == "var") doglobalvar();
            else expected("global var or function declaration");
        }
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
                    else if(m_token == "return") doreturn();
                    else if(m_token == "var") dovar();
                    match(";");
                }
            }
            else
            {
                std::string name = getName();
                if(m_token == "=") assignment(name);
                else if(m_token == "(") callFunction(name, false);
                else expected("statement");
                match(";");
            }
        }
        match("}");
    }

    void condition();
    void doif(std::string breakLabel, std::string continueLabel);
    void dowhile();
    void dobreak(std::string breakLabel);
    void docontinue(std::string continueLabel);
    void doreturn();

    inline void dovar()
    {
        match("var");
        std::string name = getName();
        m_localvars.push_back(name);

        if(m_token == "=")
            assignment(name);
    }

    inline void doglobalvar()
    {
        match("var");
        std::string name = getName();
        m_globalvars.push_back(name);
        match(";");

        write("w:\t" + name + "\n");
    }

    inline std::string toString(int val)   const { std::stringstream ss; ss << val; return ss.str(); }
};

#endif /* TRANSLATOR_H_ */
