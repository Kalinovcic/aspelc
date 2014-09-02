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
 * File: acomp.cpp
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include <cstdlib>

#include <iostream>
#include <fstream>

#include "translator.h"
#include "scanner.h"

int main()
{
    std::ifstream in("test.aspel", std::ios::in | std::ios::binary);

    LexicalScanner scanner(in);

    AspelTranslator aspTrans(scanner, std::cout);
	aspTrans.testf();

	return EXIT_SUCCESS;
}
