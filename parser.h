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

#include <stdlib.h>

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
    static int substr(char*, int, int, char*, int);
    static int clean(char[], int, char*);

    int hex_to_dec(char[], int);

    static void urldecode(char[], char*);
    static void print_error(char[], int, char[], int);
    static void exception(char[], char[], int);

    char* __PREFIX_get_resource(char[]);
    char* __PREFIX_get_operator(char[]);
    char* __PREFIX_get_filter(char[]);
    char* __PREFIX_get_conjunctive(char[], int*);

    static void validate_resource(char[], int, char[], int, const struct Statement*);
    static void validate_resource_type(char[], int, char[], int, const struct Statement*);
    static void validate_operator(char[], int, char[], int, const struct Statement*);
    static void validate_filter(char[], int, char*, char[], int, const struct Statement*);
    static void validate_conjunctive(char[], int, char[], int, const struct Statement*);

    static void validate_var(char[], int, char[], int, const struct Statement*);
    static void validate_string(char[], int, char[], int, const struct Statement*);
    static void validate_int(char[], int, char[], int, const struct Statement*);
    static void validate_double(char[], int, char[], int, const struct Statement*);
    static void validate_list(char[], int, char*, char[], int, const struct Statement*);
    static void validate_list_string(char[], int, char[], int, const struct Statement*);
    static void validate_list_double(char[], int, char[], int, const struct Statement*);
    static void validate_list_int(char[], int, char[], int, const struct Statement*);
     
#endif
