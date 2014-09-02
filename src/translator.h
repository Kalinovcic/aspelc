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

#include <stdint.h>

class AspelTranslator
{
public:
	AspelTranslator(std::istream& in, std::ostream& out);
	~AspelTranslator();
private:
	char m_look;

	std::istream& m_in;
	std::ostream& m_out;

	inline void nextChar() { m_in >> m_look; }
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
	int64_t getInteger();

	inline void write(std::string cont) { m_out << cont; }
	inline void writeln(std::string cont) { m_out << cont << "\n"; }
};

#endif /* TRANSLATOR_H_ */
