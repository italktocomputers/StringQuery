/*
The MIT License (MIT)

Copyright (c) 2015 Andrew Schools

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "extraction.h"
#include "validation.h"
#include "library.h"

#ifndef PARSER_H
#define PARSER_H

    struct Statement {
        char* resource;
        char* type;
        char* operator;
        char* filter;
        char* filter_type;
        char* conjunctive;
    };
     
    static int parse(char[], int*, struct Statement*[], int*);
    static int get_statement(char[], int*, struct Statement*[], int*);
    static int is_end(char[], int*);
    
    static int clean(char[], int, char*);

    
    static void print_error(char[], int, char[], int);
    static void exception(char[], char[], int);

    
    
    int __PREFIX_get_filter_type(char[]);
     
#endif
