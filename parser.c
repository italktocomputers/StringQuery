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


#include "parser.h"

static int is_end(char code[], int* cursor) {
    if (code[*cursor+1] != '\0') {
        // We are not at the end of the code so we have more to process.
        // Next stop: get_resource()
        (*cursor)++;
        return 1;
    }
    
    // No more code to process since we reached a '\0'
    return 0;
}

void print_error(char code[], int cursor, char msg[], int exit_code) {
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
    int length = substr(code, start, start+49, short_code, 50);
    
    // Error message
    printf("ERROR: %s at character %i\n%s\n", msg, cursor+1, short_code);
    
    // Point out where error occured
    for (; i<cursor-start; i++)
        putc('-', stdout);
        
    printf("^\n");
    
    exit(exit_code);
}

void exception(char msg[], char func[], int exit_code) {
    printf("Exception: %s @ Function: %s\n", msg, func);
    exit(exit_code); 
}



int clean(char value[], int length, char* str) {
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

int get_statement(char code[], int* cursor, struct Statement* sts[], int* sts_index) {
    struct Statement* pst = (struct Statement*)malloc(sizeof(struct Statement));
    char* type = (char*)malloc(sizeof(char[6]));
    __PREFIX_get_resource(code);
    __PREFIX_get_operator(code);
    __PREFIX_get_filter(code);
    
    int end = is_end(code, cursor);
    
    if (end != 0)
        __PREFIX_get_conjunctive(code);
    
    sts[(*sts_index)++] = pst;
    
    return end;
}

int parse(char code[], int* cursor, struct Statement* sts[], int* sts_index) {
    int length;
    int new_length;
    
    length = strlen(code);    
    new_length = clean(code, length, code);
    
    while (get_statement(code, cursor, sts, sts_index) != 0);
}
