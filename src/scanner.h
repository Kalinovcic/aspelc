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
#include <string>

class LexicalScanner
{
public:
    LexicalScanner(std::istream& in): m_in(in) {}
    virtual ~LexicalScanner() {}

    virtual std::string scan() { return ""; }
    virtual int getLine() { return 0; }
protected:
    std::istream& m_in;
};

#endif /* SCANNER_H_ */
