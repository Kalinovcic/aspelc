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
 * Description: Contains the main function.
 * Author: Lovro Kalinovcic
 * 
 */

#include <cstdlib>

#include <iostream>
#include <fstream>

#include "translator.h"
#include "scanner.h"

bool endsWith(std::string const& str, std::string const& ending)
{
    if(str.length() >= ending.length())
        return str.compare(str.length() - ending.length(), ending.length(), ending) == 0;
    return false;
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cerr << "error: no input files\n";
        exit(1);
    }

    std::string sourcePath = argv[1];
    if(!endsWith(sourcePath, ".aspel"))
    {
        std::cerr << "error: invalid extension, expected \".aspel\"\n";
        exit(1);
    }

    std::string outputPath = sourcePath.substr(0, sourcePath.length() - 6) + ".aml";

    std::ifstream in(sourcePath.c_str(), std::ios::in | std::ios::binary);
    std::ofstream out(outputPath.c_str(), std::ios::out);

    LexicalScanner scanner(in);

    AspelTranslator aspTrans(scanner, out);
	aspTrans.testf();

	return EXIT_SUCCESS;
}
