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
 * File: statement.c
 * Description: 
 * Author: Lovro Kalinovcic
 * 
 */

#include "statement.h"

struct AC_statement* AC_statement_make()
{
    struct AC_statement* object = malloc(sizeof(struct AC_statement));
    return object;
}

void AC_statement_destroy(struct AC_statement* object)
{
    switch(object->type)
    {
    case AC_STATEMENT_BLOCK:
        AC_block_destroy(object->value.block.block);
        break;
    case AC_STATEMENT_ASSIGNMENT:
        AC_identifier_destroy(object->value.assignment.name);
        AC_expression_destroy(object->value.assignment.value);
        break;
    case AC_STATEMENT_IF:
        AC_expression_destroy(object->value.zif.condition);
        AC_block_destroy(object->value.zif.execblock);
        if(object->value.zif.elseblock != AC_NULL)
            AC_block_destroy(object->value.zif.elseblock);
        break;
    case AC_STATEMENT_WHILE:
        AC_expression_destroy(object->value.zwhile.condition);
        AC_block_destroy(object->value.zwhile.execblock);
        break;
    case AC_STATEMENT_BREAK:
        break;
    case AC_STATEMENT_CONTINUE:
        break;
    case AC_STATEMENT_RETURN:
        if(object->value.zreturn.value != AC_NULL)
            AC_expression_destroy(object->value.zreturn.value);
        break;
    case AC_STATEMENT_FUNCTIONCALL:
        AC_identifier_destroy(object->value.functioncall.name);
        break;
    case AC_STATEMENT_DECLARATION:
        AC_typename_destroy(object->value.declaration.type);
        if(object->value.declaration.value != AC_NULL)
            AC_expression_destroy(object->value.declaration.value);
        break;
    case AC_STATEMENT_DEALLOCATION:
        AC_identifier_destroy(object->value.deallocation.name);
        break;
    default: AC_internal_error("unexpected statement type");
    }
    free(object);
}

static void AC_statement_load_assignment(struct AC_statement* object, struct AC_scanner* scanner)
{
    object->value.assignment.name = AC_identifier_make();
    AC_identifier_load(object->value.assignment.name, scanner);

    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "=") == AC_TRUE)
    {
        AC_scanner_match(scanner, "=");
        object->value.assignment.type = AC_STATEMENT_ASSIGNMENT_VALUE;
    }
    else if(AC_token_compare_raw(AC_scanner_get(scanner, 0), ":=") == AC_TRUE)
    {
        AC_scanner_match(scanner, ":=");
        object->value.assignment.type = AC_STATEMENT_ASSIGNMENT_POINTER;
    }
    else AC_internal_error("unknown assignment type");

    object->value.assignment.value = AC_expression_make();
    AC_expression_load(object->value.assignment.value, scanner);

    AC_scanner_match(scanner, ";");
}

static void AC_statement_load_if(struct AC_statement* object, struct AC_scanner* scanner)
{
    AC_scanner_match(scanner, "if");
    object->value.zif.condition = AC_expression_make();
    object->value.zif.execblock = AC_block_make();
    object->value.zif.elseblock = AC_NULL;
    AC_expression_load(object->value.zif.condition, scanner);
    AC_block_load(object->value.zif.execblock, scanner);
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "else") == AC_TRUE)
    {
        AC_scanner_match(scanner, "else");
        object->value.zif.elseblock = AC_block_make();
        AC_block_load(object->value.zif.elseblock, scanner);
    }
}

static void AC_statement_load_while(struct AC_statement* object, struct AC_scanner* scanner)
{
    AC_scanner_match(scanner, "while");
    object->value.zwhile.condition = AC_expression_make();
    object->value.zwhile.execblock = AC_block_make();
    AC_expression_load(object->value.zwhile.condition, scanner);
    AC_block_load(object->value.zwhile.execblock, scanner);
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "else") == AC_TRUE)
    {
        AC_scanner_match(scanner, "else");
        object->value.zif.elseblock = AC_block_make();
        AC_block_load(object->value.zif.elseblock, scanner);
    }
}

static void AC_statement_load_return(struct AC_statement* object, struct AC_scanner* scanner)
{
    AC_scanner_match(scanner, "return");
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), ";") == AC_FALSE)
    {
        object->value.zreturn.value = AC_expression_make();
        AC_expression_load(object->value.zreturn.value, scanner);
    }
    AC_scanner_match(scanner, ";");
}

static void AC_statement_load_functioncall(struct AC_statement* object, struct AC_scanner* scanner)
{
    object->value.functioncall.name = AC_identifier_make();
    AC_identifier_load(object->value.functioncall.name, scanner);
    AC_scanner_match(scanner, "(");
    // TODO: properly load the parameter list
    while(AC_token_compare_raw(AC_scanner_get(scanner, 0), ")") == AC_FALSE)
        AC_scanner_next(scanner);
    AC_scanner_match(scanner, ")");
    AC_scanner_match(scanner, ";");
}

static void AC_statement_load_declaration(struct AC_statement* object, struct AC_scanner* scanner)
{
    object->value.declaration.type = AC_typename_make();
    AC_typename_load(object->value.declaration.type, scanner);
    object->value.declaration.name = AC_scanner_getword(scanner, 0);
    AC_scanner_next(scanner);
    object->value.declaration.value = AC_NULL;
    if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "=") == AC_TRUE)
    {
        AC_scanner_match(scanner, "=");
        object->value.declaration.value = AC_expression_make();
        AC_expression_load(object->value.declaration.value, scanner);
    }
    AC_scanner_match(scanner, ";");
}

static void AC_statement_load_deallocation(struct AC_statement* object, struct AC_scanner* scanner)
{
    AC_scanner_match(scanner, "free");
    object->value.deallocation.name = AC_identifier_make();
    AC_identifier_load(object->value.deallocation.name, scanner);
    AC_scanner_match(scanner, ";");
}

void AC_statement_load(struct AC_statement* object, struct AC_scanner* scanner)
{
    struct AC_token token = AC_scanner_get(scanner, 0);
    object->srcline = token.line;

    if(AC_token_compare_raw(token, "{") == AC_TRUE)
        object->type = AC_STATEMENT_BLOCK;
    else if(AC_token_compare_raw(token, "if") == AC_TRUE)
        object->type = AC_STATEMENT_IF;
    else if(AC_token_compare_raw(token, "while") == AC_TRUE)
        object->type = AC_STATEMENT_WHILE;
    else if(AC_token_compare_raw(token, "break") == AC_TRUE)
        object->type = AC_STATEMENT_BREAK;
    else if(AC_token_compare_raw(token, "continue") == AC_TRUE)
        object->type = AC_STATEMENT_CONTINUE;
    else if(AC_token_compare_raw(token, "return") == AC_TRUE)
        object->type = AC_STATEMENT_RETURN;
    else if(AC_token_compare_raw(token, "free") == AC_TRUE)
        object->type = AC_STATEMENT_DEALLOCATION;
    else
    {
        AC_uint spos = AC_scanner_getpos(scanner);

        struct AC_identifier* identifier = AC_identifier_make();
        AC_identifier_load(identifier, scanner);
        AC_identifier_destroy(identifier);

        if(AC_scanner_isword(scanner, 0) == AC_TRUE)
            object->type = AC_STATEMENT_DECLARATION;
        else if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "(") == AC_TRUE)
            object->type = AC_STATEMENT_FUNCTIONCALL;
        else if(AC_token_compare_raw(AC_scanner_get(scanner, 0), "=") == AC_TRUE
             || AC_token_compare_raw(AC_scanner_get(scanner, 0), ":=") == AC_TRUE)
            object->type = AC_STATEMENT_ASSIGNMENT;
        else
        {
            AC_report("invalid statement near line %d\n", token.line);
            AC_report_abort();
        }
        AC_scanner_setpos(scanner, spos);
    }

    switch(object->type)
    {
    case AC_STATEMENT_BLOCK:
        printf("block\n");
        object->value.block.block = AC_block_make();
        AC_block_load(object->value.block.block, scanner);
        break;
    case AC_STATEMENT_ASSIGNMENT:
        printf("assignment\n");
        AC_statement_load_assignment(object, scanner);
        break;
    case AC_STATEMENT_IF:
        printf("if\n");
        AC_statement_load_if(object, scanner);
        break;
    case AC_STATEMENT_WHILE:
        printf("while\n");
        AC_statement_load_while(object, scanner);
        break;
    case AC_STATEMENT_BREAK:
        printf("break\n");
        AC_scanner_match(scanner, "break");
        AC_scanner_match(scanner, ";");
        break;
    case AC_STATEMENT_CONTINUE:
        printf("continue\n");
        AC_scanner_match(scanner, "continue");
        AC_scanner_match(scanner, ";");
        break;
    case AC_STATEMENT_RETURN:
        printf("return\n");
        AC_statement_load_return(object, scanner);
        break;
    case AC_STATEMENT_FUNCTIONCALL:
        printf("function call\n");
        AC_statement_load_functioncall(object, scanner);
        break;
    case AC_STATEMENT_DECLARATION:
        printf("declaration\n");
        AC_statement_load_declaration(object, scanner);
        break;
    case AC_STATEMENT_DEALLOCATION:
        printf("free\n");
        AC_statement_load_deallocation(object, scanner);
        break;
    default:
        AC_internal_error("unexpected statement type");
    }
}

static void AC_statement_translate_assignment(struct AC_statement* object, struct AC_output* output, struct AC_program* program)
{

}

static void AC_statement_translate_if(struct AC_statement* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expression_translate(object->value.zif.condition, output, program);
    if(!AC_typename_isbool(type))
    {
        AC_report("if condition near line %d does not return a value of bool type\n", object->srcline);
        AC_report_abort();
    }
    AC_typename_destroy(type);

    char* label1 = AC_label_newname();
    char* label2 = AC_NULL;
    if(object->value.zif.elseblock != AC_NULL)
        label2 = AC_label_newname();

    AC_output_write(output, "ifn %s", label1);
    AC_block_translate(object->value.zif.execblock, output, program);
    if(label2 != AC_NULL)
        AC_output_write(output, "goto %s", label2);
    AC_output_writeraw(output, "%s: \n", label1);
    if(label2 != AC_NULL)
    {
        AC_block_translate(object->value.zif.elseblock, output, program);
        AC_output_writeraw(output, "%s: \n", label2);
    }

    free(label1);
    if(label2 != AC_NULL)
        free(label2);
}

static void AC_statement_translate_while(struct AC_statement* object, struct AC_output* output, struct AC_program* program)
{
    char* label1 = AC_label_newname();
    char* label2 = AC_label_newname();

    AC_output_writeraw(output, "%s: \n", label1);

    struct AC_typename* type = AC_expression_translate(object->value.zwhile.condition, output, program);
    if(!AC_typename_isbool(type))
    {
        AC_report("if condition near line %d does not return a value of bool type\n", object->srcline);
        AC_report_abort();
    }
    AC_typename_destroy(type);

    AC_output_write(output, "ifn %s", label2);

    AC_block_translate(object->value.block.block, output, program);

    AC_output_write(output, "goto %s", label1);
    AC_output_writeraw(output, "%s: \n", label2);

    free(label1);
    free(label2);
}

static void AC_statement_translate_return(struct AC_statement* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_typename* type = AC_expression_translate(object->value.zreturn.value, output, program);
    AC_typename_destroy(type);
    AC_output_write(output, "return");
}

static void AC_statement_translate_functioncall(struct AC_statement* object, struct AC_output* output, struct AC_program* program)
{
    struct AC_function* function = AC_program_findfunc(program, object->value.functioncall.name);
    if(function == AC_NULL)
    {
        AC_report("function not found near line %d\n", object->srcline);
        AC_report_abort();
    }

    // TODO: avoid using writeraw
    AC_output_writeraw(output, "             call ");
    AC_function_writensname(function, output);
    AC_output_writeraw(output, "\n");

    AC_ulong size = AC_typename_size(function->rtype, program);
    AC_output_write(output, "pop %llu", size);
}

void AC_statement_translate(struct AC_statement* object, struct AC_output* output, struct AC_program* program)
{
    switch(object->type)
    {
    case AC_STATEMENT_BLOCK: AC_block_translate(object->value.block.block, output, program); break;
    case AC_STATEMENT_ASSIGNMENT: AC_statement_translate_assignment(object, output, program); break;
    case AC_STATEMENT_IF: AC_statement_translate_if(object, output, program); break;
    case AC_STATEMENT_WHILE: AC_statement_translate_while(object, output, program); break;
    case AC_STATEMENT_BREAK: /* TODO: implement break */ break;
    case AC_STATEMENT_CONTINUE: /* TODO: implement continue */ break;
    case AC_STATEMENT_RETURN: AC_statement_translate_return(object, output, program); break;
    case AC_STATEMENT_FUNCTIONCALL: AC_statement_translate_functioncall(object, output, program); break;
    case AC_STATEMENT_DECLARATION: break;
    case AC_STATEMENT_DEALLOCATION: break;
    default: AC_internal_error("unexpected statement type");
    }
}
