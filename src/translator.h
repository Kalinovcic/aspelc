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

	inline void expression2() { expression(); }
private:
	char m_look;

	std::istream& m_in;
	std::ostream& m_out;

	inline void nextChar() { m_in.read(&m_look, 1); }
	inline void error(std::string msg) { std::cout << "\nerror: " << msg; }
	inline void abort(std::string reason) { error(reason); exit(1); }
	inline void expected(std::string item) { abort(item + " expected"); }

	inline bool isAlpha(char c) const { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
	inline bool isDigit(char c) const { return c >= '0' && c <= '9'; }

	inline void match(char x)
	{
		if(m_look == x) nextChar();
		else expected(std::string("'") + x + "'");
	}

	std::string getName();
	int32_t getI32();

	inline void write(std::string cont) { m_out << cont; }
	inline void writeln(std::string cont) { write(cont); m_out << "\n"; }

	inline void factor() { writeln(std::string("push i32 ") + toString(getI32())); }

	inline void mul() { match('*'); factor(); writeln("muli32"); }
	inline void div() { match('/'); factor(); writeln("divi32"); }

	inline void term()
	{
		factor();
		while(m_look == '*' || m_look == '/')
			switch(m_look)
			{
			case '*': mul(); break;
			case '/': div(); break;
			}
	}

	inline void add() { match('+'); term(); writeln("addi32"); }
	inline void sub() { match('-'); term(); writeln("subi32"); }

	inline void expression()
	{
		term();
		while(m_look == '+' || m_look == '-')
			switch(m_look)
			{
			case '+': add(); break;
			case '-': sub(); break;
			}
	}

	inline std::string toString(int8_t val) { std::stringstream ss; ss << val; return ss.str(); }
	inline std::string toString(int16_t val) { std::stringstream ss; ss << val; return ss.str(); }
	inline std::string toString(int32_t val) { std::stringstream ss; ss << val; return ss.str(); }
	inline std::string toString(int64_t val) { std::stringstream ss; ss << val; return ss.str(); }

	inline std::string toString(uint8_t val) { std::stringstream ss; ss << val; return ss.str(); }
	inline std::string toString(uint16_t val) { std::stringstream ss; ss << val; return ss.str(); }
	inline std::string toString(uint32_t val) { std::stringstream ss; ss << val; return ss.str(); }
	inline std::string toString(uint64_t val) { std::stringstream ss; ss << val; return ss.str(); }

	inline std::string toString(float val) { std::stringstream ss; ss << val; return ss.str(); }
	inline std::string toString(double val) { std::stringstream ss; ss << val; return ss.str(); }
};

#endif /* TRANSLATOR_H_ */
