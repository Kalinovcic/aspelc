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

#ifndef TRANSLATOR_A11_H_
#define TRANSLATOR_A11_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

#include <vector>
#include <map>

#include "../translator.h"
#include "../scanner.h"

class TranslatorA11: public Translator
{
public:
    TranslatorA11(LexicalScanner& scanner, std::ostream& out, bool wcast);
    ~TranslatorA11();

    void translate() { program(); }
private:
    enum Type
    {
        INT,
        FLOAT,
        LONG,
        DOUBLE,
        VOID
    };

    struct FunctionData
    {
        Type rtype;
        std::vector<Type> atype;

        bool forward;
    };

    typedef std::map<std::string, FunctionData>::iterator funmap_it;

    bool m_wcast;

    std::string m_token;
    int m_labelCounter;

    FunctionData m_cfun;
    std::map<std::string, FunctionData> m_functions;

    std::vector<std::string> m_globalvars;
    std::vector<std::string> m_localvars;
    std::map<std::string, Type> m_gvartype;
    std::map<std::string, Type> m_lvartype;

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
    void warning(std::string message) const;
    void warningnl(std::string message) const;
    void error(std::string message) const;
    void errornl(std::string message) const;
    void abort(std::string reason) const;
    void abortnl(std::string reason) const;
    void expected(std::string item) const;

    // input
    std::string nextToken();
    std::string getName();
    std::string getNumber();
    Type getNumberType(std::string number);
    Type getType(bool voidAllowed);
    void match(std::string x);

    // output
    void write(std::string cont);
    void writeln(std::string cont);

    // codegen
    std::string newLabel();
    void writeLabel(std::string labelName);
    Type returnType(std::string name);
    void callFunction(std::string name, bool nonVoidOnly);
    bool variableExists(std::string name);
    Type getVariableType(std::string name);
    void fetchVariable(std::string name);
    void swap(Type top, Type next);
    void convert(Type from, Type to);
    std::string getTypeName(Type type);
    void conversionWarning(Type original, Type converted);
    Type greaterType(Type type1, Type type2);
    Type stackConvert(Type top, Type next);
    void assignment(std::string name, bool inDeclaration);
    void donew();
    void dodelete();

    // type check
    bool isInteger(Type type);
    void checkInteger(Type type, std::string op);
    void checkInteger2(Type type1, Type type2, std::string op);

    // instruction
    void instrAdd(Type type);
    void instrSub(Type type);
    void instrMul(Type type);
    void instrDiv(Type type);
    void instrRem(Type type);
    void instrNeg(Type type);
    void instrShl(Type type);
    void instrShr(Type type);
    void instrLt(Type type1, Type type2);
    void instrLe(Type type1, Type type2);
    void instrGt(Type type1, Type type2);
    void instrGe(Type type1, Type type2);
    void instrEq(Type type1, Type type2);
    void instrNe(Type type1, Type type2);
    void instrBNOT(Type type);
    void instrBAND(Type type);
    void instrBXOR(Type type);
    void instrBOR(Type type);
    void instrLNOT(Type type);
    void instrLAND(Type type);
    void instrLOR(Type type);
    void instrExtr(Type type);
    void instrPush(std::string number, Type type);
    void instrLoad(std::string name, Type type);
    void instrLoadWide(std::string name, Type type);
    void instrLoadPtr(std::string name, Type type);
    void instrLoadPtrWide(std::string name, Type type);
    void instrFetch(std::string name, Type type);
    void instrFetchWide(std::string name, Type type);

    // expression
    Type exprSuff();
    Type exprPref();
    Type exprCast();
    Type exprExtr();
    Type exprMul();
    Type exprAdd();
    Type exprBShift();
    Type exprRel();
    Type exprRelEqual();
    Type exprBAND();
    Type exprBXOR();
    Type exprBOR();
    Type exprLAND();
    Type exprLOR();
    Type expression();

    void checkFunction(std::string funName);
    void function();

    inline void program()
    {
        while(m_token != "")
        {
            if(m_token == "int"
            || m_token == "float"
            || m_token == "long"
            || m_token == "double"
            || m_token == "ref") globalDeclaration();
            else function();
        }
        for(funmap_it i = m_functions.begin(); i != m_functions.end(); i++)
            if((i->second).forward)
                abort("forwarded function \"" + i->first + "\" not found");
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
                    else if(m_token == "delete") dodelete();
                    else if(m_token == "int"
                         || m_token == "float"
                         || m_token == "long"
                         || m_token == "double"
                         || m_token == "ref") declaration();
                    match(";");
                }
            }
            else
            {
                std::string name = getName();
                if(m_token == "=" || m_token == "->") assignment(name, false);
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

    inline void declaration()
    {
        Type type = getType(false);
        std::string name = getName();

        if(variableExists(name))
            abortnl("redeclaration of '" + name + "'");

        m_lvartype[name] = type;
        if(m_token == "=")
            assignment(name, true);
        m_localvars.push_back(name);
    }

    inline void globalDeclaration()
    {
        Type type = getType(false);
        std::string name = getName();
        match(";");

        write("w:\t" + name + "\n");

        m_gvartype[name] = type;
        m_globalvars.push_back(name);
    }

    inline std::string toString(int val)   const { std::stringstream ss; ss << val; return ss.str(); }
};

#endif /* TRANSLATOR_A11_H_ */
