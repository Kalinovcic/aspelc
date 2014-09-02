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
 * File: translator.cpp
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

AspelTranslator::AspelTranslator(std::istream& in, std::ostream& out)
: m_in(in), m_out(out)
{
	nextChar();
}

AspelTranslator::~AspelTranslator()
{

}

std::string AspelTranslator::getName()
{
	std::stringstream ss;
	if(!isAlpha(m_look)) expected("name");
	while(isAlpha(m_look))
	{
		ss << m_look;
		nextChar();
	}
	return ss.str();
}

int32_t AspelTranslator::getI32()
{
	if(!isDigit(m_look)) expected("integer");
	int32_t result = 0;
	while(isDigit(m_look))
	{
		result = result * 10 + (m_look - '0');
		nextChar();
	}
	return result;
}
