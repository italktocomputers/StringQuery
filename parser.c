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
#include "parser.h"
#include "library.h"
#include "extraction.h"
#include "validation.h"

void __PREFIX_print_error(char code[], int cursor, char msg[], int exit_code) {
    int i = 0;

    // We only want to show the section of code that failed
    // (50 characters or less)
    int start;

    if (cursor-49 <= 0) {
        start = 0;
    } else {
        start = cursor - 49;
    }

    char* short_code = (char*)malloc(50);
    int length = __PREFIX_substr(code, start, start+49, short_code, 50);

    // Error message
    printf("ERROR: %s at character %i\n%s\n", msg, cursor+1, short_code);

    // Point out where error occured
    for (; i<cursor-start; i++)
        putc('-', stdout);

    printf("^\n");

    exit(exit_code);
}

void __PREFIX_exception(char msg[], char func[], int exit_code) {
    printf("Exception: %s @ Function: %s\n", msg, func);
    exit(exit_code);
}

int __PREFIX_clean(char value[], int length, char* str) {
    int i = 0;
    int x = 0;
    char tmp[RESOURCE_MAX+1];

    for (; i<length; i++) {
        if (value[i] == '\n')
            continue; // Remove line breaks, even if in a string

        tmp[x++] = value[i];
    }


    tmp[x] = '\0';

    strncpy(str, tmp, RESOURCE_MAX);

    return x;
}

void __PREFIX_parse(char code[]) {
    int i;
    int length;
    int new_length;
    int st_length;
    char* new_code = (char*)malloc(CODE_BUFFER_LENGTH+1);

    length = strlen(code);
    new_length = __PREFIX_clean(code, length, new_code);

    char** sts = __PREFIX_get_statements(new_code, &st_length);

    for (i=0; i<st_length; i++) {
        char* resource_name = __PREFIX_get_resource(sts[i]);
        char* resource_type = __PREFIX_get_resource_type(sts[i]);
        char* operator = __PREFIX_get_operator(sts[i]);
        char* filter = __PREFIX_get_filter(sts[i]);
        char* conjunctive = __PREFIX_get_conjunctive(sts[i]);

        __PREFIX_validate_resource(resource_name);
        __PREFIX_validate_resource_type(resource_type);
        __PREFIX_validate_operator(operator);
        __PREFIX_validate_conjunctive(conjunctive);

        printf("Statement: %s\n", sts[i]);
        printf("Resource: %s\n", resource_name);
        printf("Resource type: %s\n", resource_type);
        printf("Operator: %s\n", operator);
        printf("Filter: %s\n", filter);
        printf("Conjunctive: %s\n", conjunctive);
    }
}
