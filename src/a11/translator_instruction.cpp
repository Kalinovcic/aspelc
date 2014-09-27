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
 * File: translator_instruction.cpp
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "translator.h"

void TranslatorA11::instrAdd(Type type)
{
    switch(type)
    {
    case INT: writeln("addi4"); break;
    case FLOAT: writeln("addf4"); break;
    case LONG: writeln("addi8"); break;
    case DOUBLE: writeln("addf8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrSub(Type type)
{
    switch(type)
    {
    case INT: writeln("subi4"); break;
    case FLOAT: writeln("subf4"); break;
    case LONG: writeln("subi8"); break;
    case DOUBLE: writeln("subf8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrMul(Type type)
{
    switch(type)
    {
    case INT: writeln("muli4"); break;
    case FLOAT: writeln("mulf4"); break;
    case LONG: writeln("muli8"); break;
    case DOUBLE: writeln("mulf8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrDiv(Type type)
{
    switch(type)
    {
    case INT: writeln("divi4"); break;
    case FLOAT: writeln("divf4"); break;
    case LONG: writeln("divi8"); break;
    case DOUBLE: writeln("divf8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrRem(Type type)
{
    switch(type)
    {
    case INT: writeln("rem4"); break;
    case LONG: writeln("rem8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrNeg(Type type)
{
    switch(type)
    {
    case INT: writeln("negi4"); break;
    case FLOAT: writeln("negf4"); break;
    case LONG: writeln("negi8"); break;
    case DOUBLE: writeln("negf8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrShl(Type type)
{
    switch(type)
    {
    case INT: writeln("shl4"); break;
    case LONG: writeln("shl8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrShr(Type type)
{
    switch(type)
    {
    case INT: writeln("shr4"); break;
    case LONG: writeln("shr8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrLt(Type type1, Type type2)
{
    switch(type1)
    {
    case INT:
        switch(type2)
        {
        case INT: writeln("lti4i4"); break;
        case FLOAT: writeln("lti4f4"); break;
        case LONG: writeln("lti4i8"); break;
        case DOUBLE: writeln("lti4f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case FLOAT:
        switch(type2)
        {
        case INT: writeln("ltf4i4"); break;
        case FLOAT: writeln("ltf4f4"); break;
        case LONG: writeln("ltf4i8"); break;
        case DOUBLE: writeln("ltf4f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case LONG:
        switch(type2)
        {
        case INT: writeln("lti8i4"); break;
        case FLOAT: writeln("lti8f4"); break;
        case LONG: writeln("lti8i8"); break;
        case DOUBLE: writeln("lti8f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case DOUBLE:
        switch(type2)
        {
        case INT: writeln("ltf8i4"); break;
        case FLOAT: writeln("ltf8f4"); break;
        case LONG: writeln("ltf8i8"); break;
        case DOUBLE: writeln("ltf8f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrLe(Type type1, Type type2)
{
    switch(type1)
    {
    case INT:
        switch(type2)
        {
        case INT: writeln("lei4i4"); break;
        case FLOAT: writeln("lei4f4"); break;
        case LONG: writeln("lei4i8"); break;
        case DOUBLE: writeln("lei4f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case FLOAT:
        switch(type2)
        {
        case INT: writeln("lef4i4"); break;
        case FLOAT: writeln("lef4f4"); break;
        case LONG: writeln("lef4i8"); break;
        case DOUBLE: writeln("lef4f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case LONG:
        switch(type2)
        {
        case INT: writeln("lei8i4"); break;
        case FLOAT: writeln("lei8f4"); break;
        case LONG: writeln("lei8i8"); break;
        case DOUBLE: writeln("lei8f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case DOUBLE:
        switch(type2)
        {
        case INT: writeln("lef8i4"); break;
        case FLOAT: writeln("lef8f4"); break;
        case LONG: writeln("lef8i8"); break;
        case DOUBLE: writeln("lef8f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrGt(Type type1, Type type2)
{
    switch(type1)
    {
    case INT:
        switch(type2)
        {
        case INT: writeln("gti4i4"); break;
        case FLOAT: writeln("gti4f4"); break;
        case LONG: writeln("gti4i8"); break;
        case DOUBLE: writeln("gti4f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case FLOAT:
        switch(type2)
        {
        case INT: writeln("gtf4i4"); break;
        case FLOAT: writeln("gtf4f4"); break;
        case LONG: writeln("gtf4i8"); break;
        case DOUBLE: writeln("gtf4f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case LONG:
        switch(type2)
        {
        case INT: writeln("gti8i4"); break;
        case FLOAT: writeln("gti8f4"); break;
        case LONG: writeln("gti8i8"); break;
        case DOUBLE: writeln("gti8f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case DOUBLE:
        switch(type2)
        {
        case INT: writeln("gtf8i4"); break;
        case FLOAT: writeln("gtf8f4"); break;
        case LONG: writeln("gtf8i8"); break;
        case DOUBLE: writeln("gtf8f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrGe(Type type1, Type type2)
{
    switch(type1)
    {
    case INT:
        switch(type2)
        {
        case INT: writeln("gei4i4"); break;
        case FLOAT: writeln("gei4f4"); break;
        case LONG: writeln("gei4i8"); break;
        case DOUBLE: writeln("gei4f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case FLOAT:
        switch(type2)
        {
        case INT: writeln("gef4i4"); break;
        case FLOAT: writeln("gef4f4"); break;
        case LONG: writeln("gef4i8"); break;
        case DOUBLE: writeln("gef4f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case LONG:
        switch(type2)
        {
        case INT: writeln("gei8i4"); break;
        case FLOAT: writeln("gei8f4"); break;
        case LONG: writeln("gei8i8"); break;
        case DOUBLE: writeln("gei8f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case DOUBLE:
        switch(type2)
        {
        case INT: writeln("gef8i4"); break;
        case FLOAT: writeln("gef8f4"); break;
        case LONG: writeln("gef8i8"); break;
        case DOUBLE: writeln("gef8f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrEq(Type type1, Type type2)
{
    switch(type1)
    {
    case INT:
        switch(type2)
        {
        case INT: writeln("eqi4i4"); break;
        case FLOAT: writeln("eqi4f4"); break;
        case LONG: writeln("eqi4i8"); break;
        case DOUBLE: writeln("eqi4f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case FLOAT:
        switch(type2)
        {
        case INT: writeln("eqf4i4"); break;
        case FLOAT: writeln("eqf4f4"); break;
        case LONG: writeln("eqf4i8"); break;
        case DOUBLE: writeln("eqf4f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case LONG:
        switch(type2)
        {
        case INT: writeln("eqi8i4"); break;
        case FLOAT: writeln("eqi8f4"); break;
        case LONG: writeln("eqi8i8"); break;
        case DOUBLE: writeln("eqi8f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case DOUBLE:
        switch(type2)
        {
        case INT: writeln("eqf8i4"); break;
        case FLOAT: writeln("eqf8f4"); break;
        case LONG: writeln("eqf8i8"); break;
        case DOUBLE: writeln("eqf8f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrNe(Type type1, Type type2)
{
    switch(type1)
    {
    case INT:
        switch(type2)
        {
        case INT: writeln("nei4i4"); break;
        case FLOAT: writeln("nei4f4"); break;
        case LONG: writeln("nei4i8"); break;
        case DOUBLE: writeln("nei4f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case FLOAT:
        switch(type2)
        {
        case INT: writeln("nef4i4"); break;
        case FLOAT: writeln("nef4f4"); break;
        case LONG: writeln("nef4i8"); break;
        case DOUBLE: writeln("nef4f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case LONG:
        switch(type2)
        {
        case INT: writeln("nei8i4"); break;
        case FLOAT: writeln("nei8f4"); break;
        case LONG: writeln("nei8i8"); break;
        case DOUBLE: writeln("nei8f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case DOUBLE:
        switch(type2)
        {
        case INT: writeln("nef8i4"); break;
        case FLOAT: writeln("nef8f4"); break;
        case LONG: writeln("nef8i8"); break;
        case DOUBLE: writeln("nef8f8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrBAND(Type type)
{
    switch(type)
    {
    case INT: writeln("band4"); break;
    case LONG: writeln("band8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrBNOT(Type type)
{
    switch(type)
    {
    case INT: writeln("bnot4"); break;
    case LONG: writeln("bnot8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrBXOR(Type type)
{
    switch(type)
    {
    case INT: writeln("bxor4"); break;
    case LONG: writeln("bxor8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrBOR(Type type)
{
    switch(type)
    {
    case INT: writeln("bor4"); break;
    case LONG: writeln("bor8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrLNOT(Type type)
{
    switch(type)
    {
    case INT: writeln("lnot4"); break;
    case LONG: writeln("lnot8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrLAND(Type type)
{
    switch(type)
    {
    case INT: writeln("land4"); break;
    case LONG: writeln("land8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrLOR(Type type)
{
    switch(type)
    {
    case INT: writeln("lor4"); break;
    case LONG: writeln("lor8"); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrPush(std::string number, Type type)
{
    switch(type)
    {
    case INT: writeln("pushi4 " + number); break;
    case FLOAT: writeln("pushf4 " + number); break;
    case LONG: writeln("pushi8 " + number); break;
    case DOUBLE: writeln("pushf8 " + number); break;
    default: abortnl("invalid number type '" + number + "'");
    }
}
void TranslatorA11::instrLoad(std::string name, Type type)
{
    switch(type)
    {
    case INT: case FLOAT: writeln("load4 " + name); break;
    case LONG: case DOUBLE: writeln("load8 " + name); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrLoadWide(std::string name, Type type)
{
    switch(type)
    {
    case INT: case FLOAT: writeln("loadwide4 " + name); break;
    case LONG: case DOUBLE: writeln("loadwide8 " + name); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrFetch(std::string name, Type type)
{
    switch(type)
    {
    case INT: case FLOAT: writeln("fetch4 " + name); break;
    case LONG: case DOUBLE: writeln("fetch8 " + name); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrFetchWide(std::string name, Type type)
{
    switch(type)
    {
    case INT: case FLOAT: writeln("fetchwide4 " + name); break;
    case LONG: case DOUBLE: writeln("fetchwide8 " + name); break;
    default: abortnl("invalid use of types");
    }
}
