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
    case INT: writeln("remi4"); break;
    case LONG: writeln("remi8"); break;
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
void TranslatorA11::instrCmp(Type type1, Type type2)
{
    switch(type1)
    {
    case INT:
        switch(type2)
        {
        case INT: writeln("icmp4"); break;
        case FLOAT: writeln("ficmpr4"); break;
        case LONG: convert(INT, LONG); writeln("icmp8"); break;
        case DOUBLE: convert(INT, LONG); writeln("ficmpr8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case FLOAT:
        switch(type2)
        {
        case INT: writeln("ficmp4"); break;
        case FLOAT: writeln("fcmp4"); break;
        case LONG: convert(FLOAT, DOUBLE); writeln("ficmp8"); break;
        case DOUBLE: convert(FLOAT, DOUBLE); writeln("fcmp8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case LONG:
        switch(type2)
        {
        case INT: stackConvert(LONG, INT); writeln("icmp8"); break;
        case FLOAT: stackConvert(DOUBLE, FLOAT); writeln("ficmpr8"); break;
        case LONG: writeln("icmp8"); break;
        case DOUBLE: writeln("ficmpr8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    case DOUBLE:
        switch(type2)
        {
        case INT: stackConvert(LONG, INT); writeln("ficmp8"); break;
        case FLOAT: stackConvert(DOUBLE, FLOAT); writeln("fcmp8"); break;
        case LONG: writeln("ficmp8"); break;
        case DOUBLE: writeln("fcmp8"); break;
        default: abortnl("invalid use of types");
        }
        break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrLt(Type type1, Type type2)
{
    instrCmp(type1, type2);
    writeln("ltnl");
}
void TranslatorA11::instrLe(Type type1, Type type2)
{
    instrCmp(type1, type2);
    writeln("lenl");
}
void TranslatorA11::instrGt(Type type1, Type type2)
{
    instrCmp(type1, type2);
    writeln("gtnl");
}
void TranslatorA11::instrGe(Type type1, Type type2)
{
    instrCmp(type1, type2);
    writeln("genl");
}
void TranslatorA11::instrEq(Type type1, Type type2)
{
    instrCmp(type1, type2);
    writeln("eqnl");
}
void TranslatorA11::instrNe(Type type1, Type type2)
{
    instrCmp(type1, type2);
    writeln("nenl");
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
void TranslatorA11::instrExtr(Type type)
{
    switch(type)
    {
    case INT: case FLOAT: writeln("extr4"); break;
    case LONG: case DOUBLE: writeln("extr8"); break;
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
void TranslatorA11::instrRefLoad(std::string name, Type type)
{
    switch(type)
    {
    case INT: case FLOAT: writeln("refl4 " + name); break;
    case LONG: case DOUBLE: writeln("refl8 " + name); break;
    default: abortnl("invalid use of types");
    }
}
void TranslatorA11::instrRefLoadWide(std::string name, Type type)
{
    switch(type)
    {
    case INT: case FLOAT: writeln("reflwide4 " + name); break;
    case LONG: case DOUBLE: writeln("reflwide8 " + name); break;
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
