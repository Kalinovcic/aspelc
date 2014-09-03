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
 * File: scanner.h
 * Description: Contains the LexicalScanner class, used for lexical scanning.
 * Author: Lovro Kalinovcic
 * 
 */

#ifndef SCANNER_H_
#define SCANNER_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

#include <stdint.h>

class LexicalScanner
{
public:
    LexicalScanner(std::istream& in);
    ~LexicalScanner();

    inline std::string scan()
    {
        if(m_look == -1)
            return "";
        if(isAlpha(m_look))
            return getName();
        if(isDigit(m_look))
            return getNumber();
        if(isOp(m_look))
            return getOp();
        char cstr[2] = { m_look, 0 };
        std::string result = std::string(cstr);
        nextChar();
        skipWhite();
        return result;
    }

    inline int getLine() { return m_line; }
private:
    int m_line;
    char m_look;

    std::istream& m_in;

    inline void nextChar() { m_in.read(&m_look, 1); if(m_in.eof()) m_look = -1; }
    inline void error(std::string msg)     const { std::cout << "\nerror: " << msg; }
    inline void abort(std::string reason)  const { error(reason); exit(1); }
    inline void expected(std::string item) const { abort(item + " expected near line " + toString(m_line)); }

    inline bool isAlpha(char c) const { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
    inline bool isDigit(char c) const { return c >= '0' && c <= '9'; }
    inline bool isOp(char c) const { return c == '+' || c == '-' || c == '*' || c == '/' || c == '<' || c == '>' || c == '&' || c == '|' || c == '='; }
    inline bool isAlnum(char c) const { return isAlpha(c) || isDigit(c); }
    inline bool isWhite(char c) const { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

    inline void skipWhite()
    {
        while(isWhite(m_look))
        {
            if(m_look == '\n')
                m_line++;
            nextChar();
        }
    }

    inline std::string getName()
    {
        std::stringstream ss;
        if(!isAlpha(m_look)) expected("name");
        while(isAlnum(m_look))
        {
            ss << m_look;
            nextChar();
        }
        skipWhite();
        return ss.str();
    }

    inline std::string getNumber()
    {
        std::stringstream ss;
        if(!isDigit(m_look)) expected("number");
        while(isDigit(m_look))
        {
            ss << m_look;
            nextChar();
        }
        skipWhite();
        return ss.str();
    }

    inline std::string getOp()
    {
        std::stringstream ss;
        if(!isOp(m_look)) expected("operator");
        while(isOp(m_look))
        {
            ss << m_look;
            nextChar();
        }
        skipWhite();
        return ss.str();
    }

    inline std::string toString(int val)   const { std::stringstream ss; ss << val; return ss.str(); }
};

#endif /* SCANNER_H_ */
