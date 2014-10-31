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
 * File: expression.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "expression.h"

void AC_expr_level1_make(struct AC_expr_level1* object, struct AC_scanner* scanner)
{
    object->srcline = AC_scanner_get(scanner, 0).line;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "new") == AC_TRUE)
    {
        object->type = AC_EXPR_LEVEL1_NEW;
        AC_scanner_match(scanner, "new");
        object->value.new.type = AC_typename_make();
        AC_typename_load(object->value.new.type, scanner);
        object->value.new.size = AC_NULL;
        if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "[") == AC_TRUE)
        {
            AC_scanner_match(scanner, "[");
            object->value.new.size = AC_expression_make();
            AC_expression_load(object->value.new.size, scanner);
            AC_scanner_match(scanner, "]");
        }
    }
    else if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "size") == AC_TRUE)
    {
        object->type = AC_EXPR_LEVEL1_SIZE;
        AC_scanner_match(scanner, "size");
        AC_scanner_match(scanner, "(");
        object->value.size = AC_typename_make();
        AC_typename_load(object->value.size, scanner);
        AC_scanner_match(scanner, ")");
    }
    else if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "(") == AC_TRUE)
    {
        object->type = AC_EXPR_LEVEL1_EXPRESSION;
        AC_scanner_match(scanner, "(");
        object->value.expression = AC_expression_make();
        AC_expression_load(object->value.expression, scanner);
        AC_scanner_match(scanner, ")");
    }
    else if(AC_scanner_isword(scanner, 0))
    {
        struct AC_token token = AC_scanner_get(scanner, 0);
        if(AC_token_compare_raw(token, "true") == AC_TRUE
        || AC_token_compare_raw(token, "false") == AC_TRUE
        || AC_token_compare_raw(token, "null") == AC_TRUE)
        {
            object->type = AC_EXPR_LEVEL1_LITERAL;
            object->value.literal = token;
            AC_scanner_next(scanner);
        }
        else
        {
            object->type = AC_EXPR_LEVEL1_VAR;
            object->value.name = AC_identifier_make();
            AC_identifier_load(object->value.name, scanner);
            if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "(") == AC_TRUE)
            {
                object->type = AC_EXPR_LEVEL1_FUNC;
                AC_scanner_match(scanner, "(");
                // TODO: read function arguments
                AC_scanner_match(scanner, ")");
            }
        }
    }
    else
    {
        object->type = AC_EXPR_LEVEL1_LITERAL;
        object->value.literal = AC_scanner_get(scanner, 0);
        AC_scanner_next(scanner);
    }
}

void AC_expr_level2_make(struct AC_expr_level2* object, struct AC_scanner* scanner)
{
    object->srcline = AC_scanner_get(scanner, 0).line;
    object->type = AC_EXPR_LEVEL2_NOP;
    AC_bool ispos = AC_token_compare_raw(AC_scanner_get(scanner, 0), "+");
    AC_bool isneg = AC_token_compare_raw(AC_scanner_get(scanner, 0), "-");
    AC_bool islnot = AC_token_compare_raw(AC_scanner_get(scanner, 0), "!");
    AC_bool isbnot = AC_token_compare_raw(AC_scanner_get(scanner, 0), "~");
    if(ispos == AC_TRUE)
    {
        AC_scanner_match(scanner, "+");
    }
    else if(isneg == AC_TRUE)
    {
        AC_scanner_match(scanner, "-");
        object->type = AC_EXPR_LEVEL2_NEGATE;
    }
    else if(islnot == AC_TRUE)
    {
        AC_scanner_match(scanner, "!");
        object->type = AC_EXPR_LEVEL2_LNOT;
    }
    else if(isbnot == AC_TRUE)
    {
        AC_scanner_match(scanner, "~");
        object->type = AC_EXPR_LEVEL2_BNOT;
    }

    AC_expr_level1_make(&object->value, scanner);
}

void AC_expr_level3_make(struct AC_expr_level3* object, struct AC_scanner* scanner)
{
    AC_expr_level2_make(&object->value, scanner);
    object->type = AC_EXPR_LEVEL3_NOP;
}

void AC_expr_level4_make(struct AC_expr_level4* object, struct AC_scanner* scanner)
{
    object->type = AC_EXPR_LEVEL4_NOP;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "*"))
    {
        object->srcline = AC_scanner_get(scanner, 0).line;
        AC_scanner_match(scanner, "*");
        object->type = AC_EXPR_LEVEL4_DEREFERENCE;
    }
    AC_expr_level3_make(&object->value, scanner);
}

void AC_expr_level5_make(struct AC_expr_level5* object, struct AC_scanner* scanner)
{
    AC_expr_level4_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL5_NOP;
    object->next = AC_NULL;
    AC_bool ismul = AC_token_compare_raw(AC_scanner_get(scanner, 0), "*");
    AC_bool isdiv = AC_token_compare_raw(AC_scanner_get(scanner, 0), "/");
    AC_bool isrem = AC_token_compare_raw(AC_scanner_get(scanner, 0), "%");
    if(ismul == AC_TRUE || isdiv == AC_TRUE || isrem == AC_TRUE)
    {
        object->srcline = AC_scanner_get(scanner, 0).line;
        if(ismul)
        {
            AC_scanner_match(scanner, "*");
            object->type = AC_EXPR_LEVEL5_MULTIPLY;
        }
        else if(isdiv)
        {
            AC_scanner_match(scanner, "/");
            object->type = AC_EXPR_LEVEL5_DIVIDE;
        }
        else if(isrem)
        {
            AC_scanner_match(scanner, "%");
            object->type = AC_EXPR_LEVEL5_REMAINDER;
        }
        object->next = malloc(sizeof(struct AC_expr_level5));
        AC_expr_level5_make(object->next, scanner);
    }
}

void AC_expr_level6_make(struct AC_expr_level6* object, struct AC_scanner* scanner)
{
    AC_expr_level5_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL6_NOP;
    object->next = AC_NULL;
    AC_bool isadd = AC_token_compare_raw(AC_scanner_get(scanner, 0), "+");
    AC_bool issub = AC_token_compare_raw(AC_scanner_get(scanner, 0), "-");
    if(isadd == AC_TRUE || issub == AC_TRUE)
    {
        object->srcline = AC_scanner_get(scanner, 0).line;
        if(isadd)
        {
            AC_scanner_match(scanner, "+");
            object->type = AC_EXPR_LEVEL6_ADD;
        }
        else if(issub)
        {
            AC_scanner_match(scanner, "-");
            object->type = AC_EXPR_LEVEL6_SUBTRACT;
        }
        object->next = malloc(sizeof(struct AC_expr_level6));
        AC_expr_level6_make(object->next, scanner);
    }
}

void AC_expr_level7_make(struct AC_expr_level7* object, struct AC_scanner* scanner)
{
    AC_expr_level6_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL7_NOP;
    object->next = AC_NULL;
    AC_bool islsft = AC_token_compare_raw(AC_scanner_get(scanner, 0), "<<");
    AC_bool isrsft = AC_token_compare_raw(AC_scanner_get(scanner, 0), ">>");
    AC_bool islrot = AC_token_compare_raw(AC_scanner_get(scanner, 0), "<<<");
    AC_bool isrrot = AC_token_compare_raw(AC_scanner_get(scanner, 0), ">>>");
    if(islsft == AC_TRUE || isrsft == AC_TRUE || islrot == AC_TRUE || isrrot == AC_TRUE)
    {
        object->srcline = AC_scanner_get(scanner, 0).line;
        if(islsft)
        {
            object->type = AC_EXPR_LEVEL7_SHIFTLEFT;
            AC_scanner_match(scanner, "<<");
        }
        else if(isrsft)
        {
            object->type = AC_EXPR_LEVEL7_SHIFTRIGHT;
            AC_scanner_match(scanner, ">>");
        }
        else if(islrot)
        {
            object->type = AC_EXPR_LEVEL7_ROTATELEFT;
            AC_scanner_match(scanner, "<<<");
        }
        else if(isrrot)
        {
            object->type = AC_EXPR_LEVEL7_ROTATERIGHT;
            AC_scanner_match(scanner, ">>>");
        }
        object->next = malloc(sizeof(struct AC_expr_level7));
        AC_expr_level7_make(object->next, scanner);
    }
}

void AC_expr_level8_make(struct AC_expr_level8* object, struct AC_scanner* scanner)
{
    AC_expr_level7_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL8_NOP;
    object->next = AC_NULL;
    AC_bool islt = AC_token_compare_raw(AC_scanner_get(scanner, 0), "<");
    AC_bool isle = AC_token_compare_raw(AC_scanner_get(scanner, 0), "<=");
    AC_bool isgt = AC_token_compare_raw(AC_scanner_get(scanner, 0), ">");
    AC_bool isge = AC_token_compare_raw(AC_scanner_get(scanner, 0), ">=");
    if(islt == AC_TRUE || isle == AC_TRUE || isgt == AC_TRUE || isge == AC_TRUE)
    {
        object->srcline = AC_scanner_get(scanner, 0).line;
        if(islt)
        {
            object->type = AC_EXPR_LEVEL8_LESS;
            AC_scanner_match(scanner, "<");
        }
        else if(isle)
        {
            object->type = AC_EXPR_LEVEL8_LESSEQUAL;
            AC_scanner_match(scanner, "<=");
        }
        else if(isgt)
        {
            object->type = AC_EXPR_LEVEL8_GREATER;
            AC_scanner_match(scanner, ">");
        }
        else if(isge)
        {
            object->type = AC_EXPR_LEVEL8_GREATEREQUAL;
            AC_scanner_match(scanner, ">=");
        }
        object->next = malloc(sizeof(struct AC_expr_level8));
        AC_expr_level8_make(object->next, scanner);
    }
}

void AC_expr_level9_make(struct AC_expr_level9* object, struct AC_scanner* scanner)
{
    AC_expr_level8_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL9_NOP;
    object->next = AC_NULL;
    AC_bool iseql = AC_token_compare_raw(AC_scanner_get(scanner, 0), "==");
    AC_bool isneql = AC_token_compare_raw(AC_scanner_get(scanner, 0), "!=");
    if(iseql == AC_TRUE || isneql == AC_TRUE)
    {
        object->srcline = AC_scanner_get(scanner, 0).line;
        if(iseql)
        {
            object->type = AC_EXPR_LEVEL9_EQUAL;
            AC_scanner_match(scanner, "==");
        }
        else if(isneql)
        {
            object->type = AC_EXPR_LEVEL9_NOTEQUAL;
            AC_scanner_match(scanner, "!=");
        }
        object->next = malloc(sizeof(struct AC_expr_level9));
        AC_expr_level9_make(object->next, scanner);
    }
}

void AC_expr_level10_make(struct AC_expr_level10* object, struct AC_scanner* scanner)
{
    AC_expr_level9_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL10_NOP;
    object->next = AC_NULL;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "&") == AC_TRUE)
    {
        object->srcline = AC_scanner_get(scanner, 0).line;
        object->type = AC_EXPR_LEVEL10_BAND;
        object->next = malloc(sizeof(struct AC_expr_level10));
        AC_scanner_match(scanner, "&");
        AC_expr_level10_make(object->next, scanner);
    }
}

void AC_expr_level11_make(struct AC_expr_level11* object, struct AC_scanner* scanner)
{
    AC_expr_level10_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL11_NOP;
    object->next = AC_NULL;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "^") == AC_TRUE)
    {
        object->srcline = AC_scanner_get(scanner, 0).line;
        object->type = AC_EXPR_LEVEL11_BXOR;
        object->next = malloc(sizeof(struct AC_expr_level11));
        AC_scanner_match(scanner, "^");
        AC_expr_level11_make(object->next, scanner);
    }
}

void AC_expr_level12_make(struct AC_expr_level12* object, struct AC_scanner* scanner)
{
    AC_expr_level11_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL12_NOP;
    object->next = AC_NULL;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "|") == AC_TRUE)
    {
        object->srcline = AC_scanner_get(scanner, 0).line;
        object->type = AC_EXPR_LEVEL12_BOR;
        object->next = malloc(sizeof(struct AC_expr_level12));
        AC_scanner_match(scanner, "|");
        AC_expr_level12_make(object->next, scanner);
    }
}

void AC_expr_level13_make(struct AC_expr_level13* object, struct AC_scanner* scanner)
{
    AC_expr_level12_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL13_NOP;
    object->next = AC_NULL;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "&&") == AC_TRUE)
    {
        object->srcline = AC_scanner_get(scanner, 0).line;
        object->type = AC_EXPR_LEVEL13_LAND;
        object->next = malloc(sizeof(struct AC_expr_level13));
        AC_scanner_match(scanner, "&&");
        AC_expr_level13_make(object->next, scanner);
    }
}

void AC_expr_level14_make(struct AC_expr_level14* object, struct AC_scanner* scanner)
{
    AC_expr_level13_make(&object->low, scanner);
    object->type = AC_EXPR_LEVEL14_NOP;
    object->next = AC_NULL;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "||") == AC_TRUE)
    {
        object->srcline = AC_scanner_get(scanner, 0).line;
        object->type = AC_EXPR_LEVEL14_LOR;
        object->next = malloc(sizeof(struct AC_expr_level14));
        AC_scanner_match(scanner, "||");
        AC_expr_level14_make(object->next, scanner);
    }
}

void AC_expr_level1_destroy(struct AC_expr_level1* object)
{
    switch(object->type)
    {
    case AC_EXPR_LEVEL1_LITERAL:
        break;
    case AC_EXPR_LEVEL1_VAR:
        AC_identifier_destroy(object->value.name);
        break;
    case AC_EXPR_LEVEL1_FUNC:
        AC_identifier_destroy(object->value.name);
        break;
    case AC_EXPR_LEVEL1_EXPRESSION:
        AC_expression_destroy(object->value.expression);
        break;
    case AC_EXPR_LEVEL1_NEW:
        if(object->value.new.size != AC_NULL)
            AC_expression_destroy(object->value.new.size);
        AC_typename_destroy(object->value.new.type);
        break;
    case AC_EXPR_LEVEL1_SIZE:
        AC_typename_destroy(object->value.size);
        break;
    }
}

void AC_expr_level2_destroy(struct AC_expr_level2* object)
{
    AC_expr_level1_destroy(&object->value);
}

void AC_expr_level3_destroy(struct AC_expr_level3* object)
{
    AC_expr_level2_destroy(&object->value);
}

void AC_expr_level4_destroy(struct AC_expr_level4* object)
{
    AC_expr_level3_destroy(&object->value);
}

void AC_expr_level5_destroy(struct AC_expr_level5* object)
{
    AC_expr_level4_destroy(&object->low);
    if(object->type != AC_EXPR_LEVEL5_NOP)
    {
        AC_expr_level5_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level6_destroy(struct AC_expr_level6* object)
{
    AC_expr_level5_destroy(&object->low);
    if(object->type != AC_EXPR_LEVEL6_NOP)
    {
        AC_expr_level6_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level7_destroy(struct AC_expr_level7* object)
{
    AC_expr_level6_destroy(&object->low);
    if(object->type != AC_EXPR_LEVEL7_NOP)
    {
        AC_expr_level7_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level8_destroy(struct AC_expr_level8* object)
{
    AC_expr_level7_destroy(&object->low);
    if(object->type != AC_EXPR_LEVEL8_NOP)
    {
        AC_expr_level8_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level9_destroy(struct AC_expr_level9* object)
{
    AC_expr_level8_destroy(&object->low);
    if(object->type != AC_EXPR_LEVEL9_NOP)
    {
        AC_expr_level9_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level10_destroy(struct AC_expr_level10* object)
{
    AC_expr_level9_destroy(&object->low);
    if(object->type == AC_EXPR_LEVEL10_BAND)
    {
        AC_expr_level10_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level11_destroy(struct AC_expr_level11* object)
{
    AC_expr_level10_destroy(&object->low);
    if(object->type == AC_EXPR_LEVEL11_BXOR)
    {
        AC_expr_level11_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level12_destroy(struct AC_expr_level12* object)
{
    AC_expr_level11_destroy(&object->low);
    if(object->type == AC_EXPR_LEVEL12_BOR)
    {
        AC_expr_level12_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level13_destroy(struct AC_expr_level13* object)
{
    AC_expr_level12_destroy(&object->low);
    if(object->type == AC_EXPR_LEVEL13_LAND)
    {
        AC_expr_level13_destroy(object->next);
        free(object->next);
    }
}

void AC_expr_level14_destroy(struct AC_expr_level14* object)
{
    AC_expr_level13_destroy(&object->low);
    if(object->type == AC_EXPR_LEVEL14_LOR)
    {
        AC_expr_level14_destroy(object->next);
        free(object->next);
    }
}

struct AC_typename* AC_expr_level1_translate(struct AC_expr_level1* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_NULL;
    switch(object->type)
    {
    case AC_EXPR_LEVEL1_LITERAL:
        type = AC_typename_make();
        type->type = AC_TYPENAME_PRIMITIVE;

        struct AC_token literal = object->value.literal;
        if(AC_token_compare_raw(literal, "true") == AC_TRUE)
        {
            AC_output_write(output, "push4 1");
            type->value.primitive = AC_PRIMITIVE_BOOL;
        }
        else if(AC_token_compare_raw(literal, "false") == AC_TRUE)
        {
            AC_output_write(output, "push4 0");
            type->value.primitive = AC_PRIMITIVE_BOOL;
        }
        else if(AC_token_compare_raw(literal, "null") == AC_TRUE)
        {
            AC_output_write(output, "push8 0");
            type->type = AC_TYPENAME_POINTER;
            type->value.pointer = AC_NULL;
        }
        else
        {
            AC_uint origlen = literal.tokenl;
            enum AC_primitive defaulttype = AC_PRIMITIVE_INT;

            if(literal.tokenl >= 2 && literal.token[0] == '0' && literal.token[1] == 'x')
            {
                // TODO: hexadecimal literals
                AC_report("hexadecimal literals are not supported");
                AC_report_abort();
            }
            else
            {
                AC_uint i;
                for(i = 0; i < literal.tokenl; i++)
                    if(literal.token[i] == '.')
                    {
                        if(defaulttype == AC_PRIMITIVE_DOUBLE)
                        {
                            AC_report("multiple decimal points in literal \"%.*s\" near line %d", literal.tokenl, literal.token, literal.line);
                            AC_report_abort();
                        }
                        defaulttype = AC_PRIMITIVE_DOUBLE;
                    }

                enum AC_primitive suffixtype = AC_PRIMITIVE_BOOL;   // the BOOL primitive type is used as "no type"
                if(literal.token[literal.tokenl - 1] == 'i' || literal.token[literal.tokenl - 1] == 'I')
                {
                    suffixtype = AC_PRIMITIVE_INT;
                    literal.tokenl--;
                }
                else if(literal.token[literal.tokenl - 1] == 'l' || literal.token[literal.tokenl - 1] == 'L')
                {
                    suffixtype = AC_PRIMITIVE_LONG;
                    literal.tokenl--;
                }

                if(literal.token[literal.tokenl - 1] == 'u' || literal.token[literal.tokenl - 1] == 'U')
                {
                    if(suffixtype == AC_PRIMITIVE_BOOL || suffixtype == AC_PRIMITIVE_INT)
                        suffixtype = AC_PRIMITIVE_UINT;
                    else if(suffixtype == AC_PRIMITIVE_LONG)
                        suffixtype = AC_PRIMITIVE_ULONG;
                }

                if(suffixtype == AC_PRIMITIVE_BOOL)
                {
                    if(literal.token[literal.tokenl - 1] == 'f' || literal.token[literal.tokenl - 1] == 'F')
                    {
                        suffixtype = AC_PRIMITIVE_FLOAT;
                        literal.tokenl--;
                    }
                    else if(literal.token[literal.tokenl - 1] == 'd' || literal.token[literal.tokenl - 1] == 'D')
                    {
                        suffixtype = AC_PRIMITIVE_DOUBLE;
                        literal.tokenl--;
                    }
                }

                if(suffixtype == AC_PRIMITIVE_BOOL)
                    suffixtype = defaulttype;
                if(suffixtype != defaulttype)
                {
                    if(defaulttype == AC_PRIMITIVE_DOUBLE && (suffixtype == AC_PRIMITIVE_INT || suffixtype == AC_PRIMITIVE_LONG
                                                           || suffixtype == AC_PRIMITIVE_UINT || suffixtype == AC_PRIMITIVE_ULONG))
                    {
                        AC_report("invalid suffix type in literal \"%.*s\" near line %d", origlen, literal.token, literal.line);
                        AC_report_abort();
                    }
                    for(i = 0; i < literal.tokenl; i++)
                        if(!((literal.token[i] >= '0' && literal.token[i] <= '9') || literal.token[i] == '.'))
                        {
                            AC_report("invalid literal \"%.*s\" near line %d", origlen, literal.token, literal.line);
                            AC_report_abort();
                        }
                }

                type->value.primitive = suffixtype;
                switch(suffixtype)
                {
                case AC_PRIMITIVE_INT: case AC_PRIMITIVE_UINT: case AC_PRIMITIVE_FLOAT:
                    AC_output_write(output, "push4 %.*s", literal.tokenl, literal.token);
                    break;
                case AC_PRIMITIVE_LONG: case AC_PRIMITIVE_ULONG: case AC_PRIMITIVE_DOUBLE:
                    AC_output_write(output, "push8 %.*s", literal.tokenl, literal.token);
                    break;
                default: AC_internal_error("unexpected type");
                }
            }
        }

        break;
    case AC_EXPR_LEVEL1_EXPRESSION:
        type = AC_expression_translate(object->value.expression, output, program);
        break;
    case AC_EXPR_LEVEL1_VAR:
        // TODO: not finished

        AC_output_write(output, "%%FETCH%%");
        type = AC_typename_make();
        type->type = AC_TYPENAME_PRIMITIVE;
        type->value.primitive = AC_PRIMITIVE_INT;
        break;
    case AC_EXPR_LEVEL1_FUNC:
        // TODO: avoid using writeraw
        AC_output_writeraw(output, "             call ");
        struct AC_function* function = AC_namespace_findfunc(program->globalns, object->value.name, 1);
        if(function == AC_NULL)
        {
            AC_report("function called in expression near line %d not found", object->srcline);
        }
        AC_function_writensname(function, output);
        AC_output_writeraw(output, "\n");

        type = AC_typename_make();
        type->type = AC_TYPENAME_PRIMITIVE;
        type->value.primitive = AC_PRIMITIVE_INT;
        break;
    case AC_EXPR_LEVEL1_NEW:
        AC_output_write(output, "push8 %d", AC_typename_size(object->value.size, program));
        if(object->value.new.size != AC_NULL)
        {
            struct AC_typename* sizetype = AC_expression_translate(object->value.new.size, output, program);
            if(!AC_typename_isinteger(sizetype))
            {
                AC_report("invalid size expression to dynamic memory allocator \"new\" near line %d\n", object->srcline);
                AC_report_abort();
            }
            if(sizetype->value.primitive != AC_PRIMITIVE_LONG || sizetype->value.primitive != AC_PRIMITIVE_ULONG)
                AC_output_write(output, "ci48");
            AC_output_write(output, "muli8");
            AC_typename_destroy(sizetype);
        }
        AC_output_write(output, "alloc");

        type = AC_typename_make();
        type->type = AC_TYPENAME_POINTER;
        type->value.pointer = object->value.new.type;
        break;
    case AC_EXPR_LEVEL1_SIZE:
        AC_output_write(output, "push8 %llu", AC_typename_size(object->value.size, program));

        type = AC_typename_make();
        type->type = AC_TYPENAME_PRIMITIVE;
        type->value.primitive = AC_PRIMITIVE_ULONG;
        break;
    }
    assert(type != AC_NULL);
    return type;
}

struct AC_typename* AC_expr_level2_translate(struct AC_expr_level2* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level1_translate(&object->value, output, program);
    if(object->type != AC_EXPR_LEVEL2_NOP)
    {
        /*
        switch(object->type)
        {
        case AC_EXPR_LEVEL2_NEGATE:
            if(!AC_)
            break;
        default:
        }
        */
    }
    return type;
}

struct AC_typename* AC_expr_level3_translate(struct AC_expr_level3* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level2_translate(&object->value, output, program);
    if(object->type != AC_EXPR_LEVEL3_NOP)
    {

    }
    return type;
}

struct AC_typename* AC_expr_level4_translate(struct AC_expr_level4* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level3_translate(&object->value, output, program);
    if(object->type != AC_EXPR_LEVEL4_NOP)
    {
        AC_output_write(output, "deref %d", AC_primitive_size(type->value.pointer->value.primitive));
        AC_typename_destroy_noptr(type);
        return type->value.pointer;
    }
    return type;
}

struct AC_typename* AC_expr_level5_translate(struct AC_expr_level5* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level4_translate(&object->low, output, program);
    if(object->type != AC_EXPR_LEVEL5_NOP)
    {
        if(AC_typename_isnumber(type) != AC_TRUE || (object->type == AC_EXPR_LEVEL5_REMAINDER && AC_typename_isinteger(type) != AC_TRUE))
        {
            if(object->type == AC_EXPR_LEVEL5_MULTIPLY)
                AC_invalid_operand(type, "binary operator multiplication", object->srcline);
            else if(object->type == AC_EXPR_LEVEL5_DIVIDE)
                AC_invalid_operand(type, "binary operator division", object->srcline);
            else if(object->type == AC_EXPR_LEVEL5_REMAINDER)
                AC_invalid_operand(type, "binary operator remainder", object->srcline);
        }

        struct AC_typename* type2 = AC_expr_level5_translate(object->next, output, program);
        if(AC_typename_isnumber(type) != AC_TRUE || (object->type == AC_EXPR_LEVEL5_REMAINDER && AC_typename_isinteger(type) != AC_TRUE))
        {
            if(object->type == AC_EXPR_LEVEL5_MULTIPLY)
                AC_invalid_operand(type2, "binary operator multiplication", object->srcline);
            else if(object->type == AC_EXPR_LEVEL5_DIVIDE)
                AC_invalid_operand(type2, "binary operator division", object->srcline);
            else if(object->type == AC_EXPR_LEVEL5_REMAINDER)
                AC_invalid_operand(type2, "binary operator remainder", object->srcline);
        }

        type = AC_typename_stackconv(type2, type, object->srcline, output);
        AC_output_write(output, "%s%c%d", (object->type == AC_EXPR_LEVEL5_MULTIPLY) ? "mul" : ((object->type == AC_EXPR_LEVEL5_DIVIDE) ? "div" : "rem"),
                                          (AC_typename_isinteger(type) == AC_TRUE) ? ((object->type == AC_EXPR_LEVEL5_DIVIDE && AC_typename_isunsigned(type)) ? 'u' : 'i') : 'f',
                                          AC_primitive_size(type->value.primitive), object->srcline);
    }
    return type;
}

struct AC_typename* AC_expr_level6_translate(struct AC_expr_level6* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level5_translate(&object->low, output, program);
    if(object->type != AC_EXPR_LEVEL6_NOP)
    {
        if(!AC_typename_isnumber(type))
        {
            if(object->type == AC_EXPR_LEVEL6_ADD)
                AC_invalid_operand(type, "binary operator addition", object->srcline);
            else if(object->type == AC_EXPR_LEVEL6_SUBTRACT)
                AC_invalid_operand(type, "binary operator subtraction", object->srcline);
        }

        struct AC_typename* type2 = AC_expr_level6_translate(object->next, output, program);
        if(!AC_typename_isnumber(type))
        {
            if(object->type == AC_EXPR_LEVEL6_ADD)
                AC_invalid_operand(type, "binary operator addition", object->srcline);
            else if(object->type == AC_EXPR_LEVEL6_SUBTRACT)
                AC_invalid_operand(type, "binary operator subtraction", object->srcline);
        }

        type = AC_typename_stackconv(type2, type, object->srcline, output);
        AC_output_write(output, "%s%c%d", (object->type == AC_EXPR_LEVEL6_ADD) ? "add" : "sub",
                                          (AC_typename_isinteger(type) == AC_TRUE) ? 'i' : 'f',
                                          AC_primitive_size(type->value.primitive), object->srcline);
    }
    return type;
}

struct AC_typename* AC_expr_level7_translate(struct AC_expr_level7* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level6_translate(&object->low, output, program);
    if(object->type != AC_EXPR_LEVEL7_NOP)
    {
        if(!AC_typename_isinteger(type))
        {
            if(object->type == AC_EXPR_LEVEL7_SHIFTLEFT)
                AC_invalid_operand(type, "bitwise operator shift left", object->srcline);
            else if(object->type == AC_EXPR_LEVEL7_SHIFTRIGHT)
                AC_invalid_operand(type, "bitwise operator shift right", object->srcline);
            else if(object->type == AC_EXPR_LEVEL7_ROTATELEFT)
                AC_invalid_operand(type, "bitwise operator rotate left", object->srcline);
            else if(object->type == AC_EXPR_LEVEL7_ROTATERIGHT)
                AC_invalid_operand(type, "bitwise operator rotate right", object->srcline);
        }

        struct AC_typename* type2 = AC_expr_level7_translate(object->next, output, program);
        if(!AC_typename_isinteger(type2))
        {
            if(object->type == AC_EXPR_LEVEL7_SHIFTLEFT)
                AC_invalid_operand(type2, "bitwise operator shift left", object->srcline);
            else if(object->type == AC_EXPR_LEVEL7_SHIFTRIGHT)
                AC_invalid_operand(type2, "bitwise operator shift right", object->srcline);
            else if(object->type == AC_EXPR_LEVEL7_ROTATELEFT)
                AC_invalid_operand(type2, "bitwise operator rotate left", object->srcline);
            else if(object->type == AC_EXPR_LEVEL7_ROTATERIGHT)
                AC_invalid_operand(type2, "bitwise operator rotate right", object->srcline);
        }

        type = AC_typename_stackconv(type2, type, object->srcline, output);

        if(object->type == AC_EXPR_LEVEL7_SHIFTLEFT)
            AC_output_write(output, "shl%d", AC_primitive_size(type->value.primitive));
        else if(object->type == AC_EXPR_LEVEL7_SHIFTRIGHT)
            if(AC_typename_isunsigned(type) == AC_TRUE)
                AC_output_write(output, "shru%d", AC_primitive_size(type->value.primitive));
            else
                AC_output_write(output, "shr%d", AC_primitive_size(type->value.primitive));
        else if(object->type == AC_EXPR_LEVEL7_ROTATELEFT)
            AC_output_write(output, "rotl%d", AC_primitive_size(type->value.primitive));
        else if(object->type == AC_EXPR_LEVEL7_ROTATERIGHT)
            AC_output_write(output, "rotr%d", AC_primitive_size(type->value.primitive));
    }
    return type;
}

struct AC_typename* AC_expr_level8_translate(struct AC_expr_level8* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level7_translate(&object->low, output, program);
    if(object->type != AC_EXPR_LEVEL8_NOP)
    {
        if(!AC_typename_isnumber(type))
        {
            if(object->type == AC_EXPR_LEVEL8_LESS)
                AC_invalid_operand(type, "relational operator less", object->srcline);
            else if(object->type == AC_EXPR_LEVEL8_LESSEQUAL)
                AC_invalid_operand(type, "relational operator less equal", object->srcline);
            else if(object->type == AC_EXPR_LEVEL8_GREATEREQUAL)
                AC_invalid_operand(type, "relational operator greater", object->srcline);
            else if(object->type == AC_EXPR_LEVEL8_GREATER)
                AC_invalid_operand(type, "relational operator greater equal", object->srcline);
        }

        struct AC_typename* type2 = AC_expr_level8_translate(object->next, output, program);
        if(!AC_typename_isnumber(type2))
        {
            if(object->type == AC_EXPR_LEVEL8_LESS)
                AC_invalid_operand(type2, "relational operator less", object->srcline);
            else if(object->type == AC_EXPR_LEVEL8_LESSEQUAL)
                AC_invalid_operand(type2, "relational operator less equal", object->srcline);
            else if(object->type == AC_EXPR_LEVEL8_GREATEREQUAL)
                AC_invalid_operand(type2, "relational operator greater", object->srcline);
            else if(object->type == AC_EXPR_LEVEL8_GREATER)
                AC_invalid_operand(type2, "relational operator greater equal", object->srcline);
        }

        type = AC_typename_stackconv(type2, type, object->srcline, output);
        AC_output_write(output, "cmp%d", AC_primitive_size(type->value.primitive));

        if(object->type == AC_EXPR_LEVEL8_LESS) AC_output_write(output, "ltnl");
        else if(object->type == AC_EXPR_LEVEL8_LESSEQUAL) AC_output_write(output, "lenl");
        else if(object->type == AC_EXPR_LEVEL8_GREATER) AC_output_write(output, "gtnl");
        else if(object->type == AC_EXPR_LEVEL8_GREATEREQUAL) AC_output_write(output, "genl");

        AC_typename_destroy(type);

        struct AC_typename* booltn = AC_typename_make();
        booltn->type = AC_TYPENAME_PRIMITIVE;
        booltn->value.primitive = AC_PRIMITIVE_BOOL;
        return booltn;
    }
    return type;
}

struct AC_typename* AC_expr_level9_translate(struct AC_expr_level9* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level8_translate(&object->low, output, program);
    if(object->type != AC_EXPR_LEVEL9_NOP)
    {
        if(!AC_typename_isnumber(type) && type->type != AC_TYPENAME_POINTER)
        {
            if(object->type == AC_EXPR_LEVEL9_EQUAL)
                AC_invalid_operand(type, "relational operator equality", object->srcline);
            else if(object->type == AC_EXPR_LEVEL9_EQUAL)
                AC_invalid_operand(type, "relational operator not equality", object->srcline);
        }
        struct AC_typename* type2 = AC_expr_level9_translate(object->next, output, program);
        if(!AC_typename_isnumber(type2) && type2->type != AC_TYPENAME_POINTER)
        {
            if(object->type == AC_EXPR_LEVEL9_EQUAL)
                AC_invalid_operand(type2, "relational operator equality", object->srcline);
            else if(object->type == AC_EXPR_LEVEL9_EQUAL)
                AC_invalid_operand(type2, "relational operator not equality", object->srcline);
        }

        if(type->type == AC_TYPENAME_POINTER && type2->type == AC_TYPENAME_POINTER)
        {
            AC_typename_destroy(type2);
            AC_output_write(output, "cmp8");
        }
        else
        {
            type = AC_typename_stackconv(type2, type, object->srcline, output);
            AC_output_write(output, "cmp%d", AC_primitive_size(type->value.primitive));
        }

        if(object->type == AC_EXPR_LEVEL9_EQUAL)
            AC_output_write(output, "eqnl");
        else AC_output_write(output, "nenl");

        AC_typename_destroy(type);

        struct AC_typename* booltn = AC_typename_make();
        booltn->type = AC_TYPENAME_PRIMITIVE;
        booltn->value.primitive = AC_PRIMITIVE_BOOL;
        return booltn;
    }
    return type;
}

struct AC_typename* AC_expr_level10_translate(struct AC_expr_level10* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level9_translate(&object->low, output, program);
    if(object->type != AC_EXPR_LEVEL10_NOP)
    {
        if(!AC_typename_isinteger(type))
            AC_invalid_operand(type, "logical operator AND", object->srcline);
        struct AC_typename* type2 = AC_expr_level10_translate(object->next, output, program);
        if(!AC_typename_isinteger(type2))
            AC_invalid_operand(type2, "logical operator AND", object->srcline);
        type = AC_typename_stackconv(type2, type, object->srcline, output);
        AC_output_write(output, "band%d", AC_primitive_size(type->value.primitive));
    }
    return type;
}

struct AC_typename* AC_expr_level11_translate(struct AC_expr_level11* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level10_translate(&object->low, output, program);
    if(object->type != AC_EXPR_LEVEL11_NOP)
    {
        if(!AC_typename_isinteger(type))
            AC_invalid_operand(type, "logical operator XOR", object->srcline);
        struct AC_typename* type2 = AC_expr_level11_translate(object->next, output, program);
        if(!AC_typename_isinteger(type2))
            AC_invalid_operand(type2, "logical operator XOR", object->srcline);
        type = AC_typename_stackconv(type2, type, object->srcline, output);
        AC_output_write(output, "bxor%d", AC_primitive_size(type->value.primitive));
    }
    return type;
}

struct AC_typename* AC_expr_level12_translate(struct AC_expr_level12* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level11_translate(&object->low, output, program);
    if(object->type != AC_EXPR_LEVEL12_NOP)
    {
        if(!AC_typename_isinteger(type))
            AC_invalid_operand(type, "logical operator OR", object->srcline);
        struct AC_typename* type2 = AC_expr_level12_translate(object->next, output, program);
        if(!AC_typename_isinteger(type2))
            AC_invalid_operand(type2, "logical operator OR", object->srcline);
        type = AC_typename_stackconv(type2, type, object->srcline, output);
        AC_output_write(output, "bor%d", AC_primitive_size(type->value.primitive));
    }
    return type;
}

struct AC_typename* AC_expr_level13_translate(struct AC_expr_level13* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level12_translate(&object->low, output, program);
    if(object->type != AC_EXPR_LEVEL13_NOP)
    {
        if(!AC_typename_isbool(type))
            AC_invalid_operand(type, "logical operator AND", object->srcline);
        struct AC_typename* type2 = AC_expr_level13_translate(object->next, output, program);
        if(!AC_typename_isbool(type2))
            AC_invalid_operand(type2, "logical operator AND", object->srcline);
        AC_output_write(output, "land4");
    }
    return type;
}

struct AC_typename* AC_expr_level14_translate(struct AC_expr_level14* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expr_level13_translate(&object->low, output, program);
    if(object->type != AC_EXPR_LEVEL14_NOP)
    {
        if(!AC_typename_isbool(type))
            AC_invalid_operand(type, "logical operator OR", object->srcline);
        struct AC_typename* type2 = AC_expr_level14_translate(object->next, output, program);
        if(!AC_typename_isbool(type2))
            AC_invalid_operand(type2, "logical operator OR", object->srcline);
        AC_output_write(output, "lor4");
    }
    return type;
}

struct AC_expression* AC_expression_make()
{
    struct AC_expression* object = malloc(sizeof(struct AC_expression));
    return object;
}

void AC_expression_destroy(struct AC_expression* object)
{
    AC_expr_level14_destroy(&object->top);
    free(object);
}

void AC_expression_load(struct AC_expression* object, struct AC_scanner* scanner)
{
    AC_expr_level14_make(&object->top, scanner);
}

struct AC_typename* AC_expression_translate(struct AC_expression* object, struct AC_output* output, struct AC_program* program)
{
    return AC_expr_level14_translate(&object->top, output, program);
}
