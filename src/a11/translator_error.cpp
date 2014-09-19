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
 * File: translator_error.cpp
 * Description: Methods from AspelTranslator for error handling.
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

void TranslatorA11::warning(std::string message) const { std::cout << "warning: " << message << "\n";}
void TranslatorA11::warningnl(std::string message) const { std::cout << "warning: " << message << " near line " << m_scanner.getLine() << "\n";}
void TranslatorA11::error(std::string message) const { std::cout << "error: " << message << "\n";}
void TranslatorA11::errornl(std::string message) const { std::cout << "error: " << message << " near line " << m_scanner.getLine() << "\n";}
void TranslatorA11::abort(std::string reason)  const { error("fatal: " + reason); exit(1); };
void TranslatorA11::abortnl(std::string reason)  const { errornl("fatal: " + reason); exit(1); };
void TranslatorA11::expected(std::string item) const { abortnl(item + " expected"); };
