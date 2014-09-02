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

#include <stdint.h>

class AspelTranslator
{
public:
    AspelTranslator(std::istream& in, std::ostream& out);
    ~AspelTranslator();

    inline void testf() { block("-", "-"); }
private:
    char m_look;
    int m_labelCounter;

    std::istream& m_in;
    std::ostream& m_out;

    inline void nextChar() { m_in.read(&m_look, 1); }
    inline void error(std::string msg)     const { std::cout << "\nerror: " << msg; }
    inline void abort(std::string reason)  const { error(reason); exit(1); }
    inline void expected(std::string item) const { abort(item + " expected"); }

    inline bool isAlpha(char c) const { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
    inline bool isDigit(char c) const { return c >= '0' && c <= '9'; }
    inline bool isAlnum(char c) const { return isAlpha(c) || isDigit(c); }
    inline bool isAddop(char c) const { return c == '+' || c == '-'; }
    inline bool isMulop(char c) const { return c == '*' || c == '/'; }
    inline bool isOrop(char c) const { return c == '|' || c == '^'; }
    inline bool isRelop(char c) const { return c == '=' || c == '#' || c == '<' || c == '>'; }
    inline bool isWhite(char c) const { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
    inline bool isBool(char c) const { return c == 'T' || c == 'F'; }

    inline void skipWhite()
    {
        while(isWhite(m_look))
            nextChar();
    }

    inline void match(char x)
    {
        if(m_look != x) expected(std::string("'") + x + "'");
        else
        {
            nextChar();
            skipWhite();
        }
    }

    inline void match(std::string x)
    {
        for(unsigned int i = 0; i < x.size(); i++)
        {
            if(m_look != x[i])
                expected("'" + x + "'");
            nextChar();
        }
        skipWhite();
    }

    std::string getWord();
    std::string getName();
    int32_t getI32();

    inline void write(std::string cont) const { m_out << cont; }
    inline void writeln(std::string cont) const { write(cont); m_out << "\n"; }

    inline std::string newLabel() { return "l" + toString(m_labelCounter++); }
    inline void writeLabel(std::string labelname) { writeln(labelname + ":"); }

    inline void identifier()
    {
        std::string name = getName();
        if(m_look == '(')
        {
            match('(');
            // TODO: parameters
            match(')');
            writeln("call " + name);
        }
        else
            writeln("fetch " + name);
    }

    inline void factor()
    {
        if(m_look == '(')
        {
            match('(');
            expression();
            match(')');
        }
        else if(isAlpha(m_look))
        {
            identifier();
        }
        else
        {
            writeln("push " + toString(getI32()));
        }
    }

    inline void signedFactor()
    {
        if(m_look == '+' || m_look == '-')
        {
            nextChar();
            if(m_look == '-')
            {
                if(isDigit(m_look))
                {
                    writeln("push -" + toString(getI32()));
                }
                else
                {
                    factor();
                    writeln("neg");
                }
            }
        }
        else
        {
            factor();
        }
    }

    inline void mul()
    {
        match('*');
        factor();
        writeln("mul");
    }

    inline void div()
    {
        match('/');
        factor();
        writeln("div");
    }

    inline void term()
    {
        signedFactor();
        while(isMulop(m_look))
            switch(m_look)
            {
            case '*': mul(); break;
            case '/': div(); break;
            }
    }

    inline void add()
    {
        match('+');
        term();
        writeln("add");
    }

    inline void sub()
    {
        match('-');
        term();
        writeln("sub");
    }

    inline void expression()
    {
        term();
        while(isAddop(m_look))
            switch(m_look)
            {
            case '+': add(); break;
            case '-': sub(); break;
            }
    }

    inline void equals()
    {
        match('=');
        expression();
        writeln("eq");
    }

    inline void notEqual()
    {
        match('#');
        expression();
        writeln("ne");
    }

    inline void less()
    {
        match('<');
        expression();
        writeln("lt");
    }

    inline void greater()
    {
        match('>');
        expression();
        writeln("gt");
    }

    inline void relation()
    {
        expression();
        if(isRelop(m_look))
            switch(m_look)
            {
            case '=': equals(); break;
            case '#': notEqual(); break;
            case '<': less(); break;
            case '>': greater(); break;
            }
    }

    inline bool getBool()
    {
        std::cout << "hey " << m_look << "\n";
        if(!isBool(m_look)) expected("boolean literal");
        bool result = m_look == 'T';
        nextChar();
        return result;
    }

    inline void boolFactor()
    {
        if(isBool(m_look))
        {
            if(getBool()) writeln("push true");
            else writeln("push false");
        }
        else relation();
    }

    inline void notFactor()
    {
        if(m_look == '!')
        {
            match('!');
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
        match('&');
        notFactor();
        writeln("and");
    }

    inline void boolTerm()
    {
        notFactor();
        while(m_look == '&')
            booland();
    }

    inline void boolor()
    {
        match('|');
        boolTerm();
        writeln("or");
    }

    inline void boolxor()
    {
        match('^');
        boolTerm();
        writeln("xor");
    }

    inline void boolExpression()
    {
        boolTerm();

        while(isOrop(m_look))
            switch(m_look)
            {
            case '|': boolor(); break;
            case '^': boolxor(); break;
            }
    }

    inline void assignment()
    {
        std::string name = getName();
        match('=');
        boolExpression();
        writeln("load " + name);
    }

    inline void block(std::string breakLabel, std::string continueLabel)
    {
        match('{');
        while(m_look != '}')
        {
            if(m_look == '?') doif(breakLabel, continueLabel);
            else if(m_look == '@') dowhile();
            else
            {
                if(m_look == '#') dobreak(breakLabel);
                else if(m_look == '$') docontinue(continueLabel);
                else assignment();
                match(';');
            }
        }
        match('}');
    }

    inline void condition()
    {
        match('(');
        writeln("push 1");
        match(')');
    }

    inline void doif(std::string breakLabel, std::string continueLabel)
    {
        match("?");
        condition();

        std::string toElse = newLabel();
        std::string toEnd;

        writeln("ifn " + toElse);

        block(breakLabel, continueLabel);

        bool hasElse = m_look == ':';

        if(hasElse)
        {
            match(':');

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
        match("@");

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
        match('#');
        if(breakLabel == "-")
            abort("no label to break to");
        writeln("goto " + breakLabel);
    }

    inline void docontinue(std::string continueLabel)
    {
        match('$');
        if(continueLabel == "-")
            abort("no label to continue from");
        writeln("goto " + continueLabel);
    }

    inline std::string toString(int8_t val)   const { std::stringstream ss; ss << val; return ss.str(); }
    inline std::string toString(int16_t val)  const { std::stringstream ss; ss << val; return ss.str(); }
    inline std::string toString(int32_t val)  const { std::stringstream ss; ss << val; return ss.str(); }
    inline std::string toString(int64_t val)  const { std::stringstream ss; ss << val; return ss.str(); }

    inline std::string toString(uint8_t val)  const { std::stringstream ss; ss << val; return ss.str(); }
    inline std::string toString(uint16_t val) const { std::stringstream ss; ss << val; return ss.str(); }
    inline std::string toString(uint32_t val) const { std::stringstream ss; ss << val; return ss.str(); }
    inline std::string toString(uint64_t val) const { std::stringstream ss; ss << val; return ss.str(); }

    inline std::string toString(float val)    const { std::stringstream ss; ss << val; return ss.str(); }
    inline std::string toString(double val)   const { std::stringstream ss; ss << val; return ss.str(); }
};

#endif /* TRANSLATOR_H_ */
