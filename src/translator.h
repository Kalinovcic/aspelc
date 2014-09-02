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

    inline void nextToken() { m_token = m_scanner.scan(); }
    inline void error(std::string msg)     const { std::cout << "\nerror: " << msg; }
    inline void abort(std::string reason)  const { error(reason); exit(1); }
    inline void expected(std::string item) const { abort(item + " expected"); }

    inline bool isAlpha(char c) const { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
    inline bool isDigit(char c) const { return c >= '0' && c <= '9'; }
    inline bool isAlnum(char c) const { return isAlpha(c) || isDigit(c); }
    inline bool isAddop(std::string c) const { return c == "+" || c == "-"; }
    inline bool isMulop(std::string c) const { return c == "*" || c == "/"; }
    inline bool isOrop(std::string c) const { return c == "|" || c == "^"; }
    inline bool isRelop(std::string c) const { return c == "=" || c == "!=" || c == "<" || c == ">"; }
    inline bool isBool(std::string c) const { return c == "true" || c == "false"; }

    inline void match(std::string x)
    {
        if(m_token != x) expected("'" + x + "'");
        nextToken();
    }

    inline std::string getName()
    {
        if(!isAlpha(m_token[0])) expected("name");
        std::string name = m_token;
        nextToken();
        return name;
    }

    inline std::string getNumber()
    {
        if(!isDigit(m_token[0])) expected("number");
        std::string number = m_token;
        nextToken();
        return number;
    }

    inline void write(std::string cont) const { m_out << cont; }
    inline void writeln(std::string cont) const { write(cont); m_out << "\n"; }

    inline std::string newLabel() { return "l" + toString(m_labelCounter++); }
    inline void writeLabel(std::string labelname) { writeln(labelname + ":"); }

    inline void identifier()
    {
        std::string name = getName();
        if(m_token == "(")
        {
            match("(");
            // TODO: parameters
            match(")");
            writeln("call " + name);
        }
        else
            writeln("fetch " + name);
    }

    inline void factor()
    {
        if(m_token == "(")
        {
            match("(");
            expression();
            match(")");
        }
        else if(isAlpha(m_token[0]))
        {
            identifier();
        }
        else
        {
            writeln("push " + getNumber());
        }
    }

    inline void signedFactor()
    {
        if(m_token == "+") nextToken();
        else if(m_token == "-")
        {
            nextToken();
            if(isDigit(m_token[0]))
            {
                writeln("push -" + getNumber());
            }
            else
            {
                factor();
                writeln("neg");
            }
        }
        else
        {
            factor();
        }
    }

    inline void mul()
    {
        match("*");
        factor();
        writeln("mul");
    }

    inline void div()
    {
        match("/");
        factor();
        writeln("div");
    }

    inline void term()
    {
        signedFactor();
        while(isMulop(m_token))
        {
            if(m_token == "*") mul();
            else if(m_token == "/") div();
        }
    }

    inline void add()
    {
        match("+");
        term();
        writeln("add");
    }

    inline void sub()
    {
        match("-");
        term();
        writeln("sub");
    }

    inline void expression()
    {
        term();
        while(isAddop(m_token))
        {
            if(m_token == "+") add();
            else if(m_token == "-") sub();
        }
    }

    inline void equals()
    {
        match("=");
        expression();
        writeln("eq");
    }

    inline void notEqual()
    {
        match("!=");
        expression();
        writeln("ne");
    }

    inline void less()
    {
        match("<");
        expression();
        writeln("lt");
    }

    inline void greater()
    {
        match(">");
        expression();
        writeln("gt");
    }

    inline void relation()
    {
        expression();
        if(isRelop(m_token))
        {
            if(m_token == "=") equals();
            else if(m_token == "!=") notEqual();
            else if(m_token == "<") less();
            else if(m_token == ">") greater();
        }
    }

    inline bool getBool()
    {
        if(!isBool(m_token)) expected("boolean literal");
        bool result = m_token == "true";
        nextToken();
        return result;
    }

    inline void boolFactor()
    {
        if(isBool(m_token))
        {
            if(getBool()) writeln("push true");
            else writeln("push false");
        }
        else relation();
    }

    inline void notFactor()
    {
        if(m_token == "!")
        {
            match("!");
            boolFactor();
            writeln("not");
        }
        else
        {
            boolFactor();
        }
    }

    inline void booland()
    {
        match("&");
        notFactor();
        writeln("and");
    }

    inline void boolTerm()
    {
        notFactor();
        while(m_token == "&")
            booland();
    }

    inline void boolor()
    {
        match("|");
        boolTerm();
        writeln("or");
    }

    inline void boolxor()
    {
        match("^");
        boolTerm();
        writeln("xor");
    }

    inline void boolExpression()
    {
        boolTerm();

        while(isOrop(m_token))
        {
            if(m_token == "|") boolor();
            else if(m_token == "^") boolxor();
        }
    }

    inline void assignment()
    {
        std::string name = getName();
        match("=");
        boolExpression();
        writeln("load " + name);
    }

    inline void block(std::string breakLabel, std::string continueLabel)
    {
        match("{");
        while(m_token != "}")
        {
            if(m_token == "if") doif(breakLabel, continueLabel);
            else if(m_token == "while") dowhile();
            else
            {
                if(m_token == "break") dobreak(breakLabel);
                else if(m_token == "continue") docontinue(continueLabel);
                else assignment();
                match(";");
            }
        }
        match("}");
    }

    inline void condition()
    {
        match("(");
        boolExpression();
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
