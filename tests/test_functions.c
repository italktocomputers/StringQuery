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
#include "test_functions.h"

void print_dots(int l) {
    int i=0;
    int n_dots = NUM_SPACES_TO_RIGHT - l;
    
    for (; i<n_dots; i++) {
        printf(".");
    }
}

void pass(char* test_name) {
    int l = strlen(test_name);
    
    printf("%s", test_name);
    printf("\033[0;32m");
    print_dots(l);
    printf("\033[0;32m[Passed!]\033[0m\n");
}

void fail(char* test_name, char* expected, char* actual) {
    int l = strlen(test_name);
    
    printf("%s", test_name);
    printf("\033[0;31m");
    print_dots(l);
    printf("[Failed!]\n");
    printf("Expecting %s but got %s\033[0m\n", expected, actual);
}

void test_string(char* test_name, char* expected, char* actual) {
    if (strcmp(expected, actual) == 0) {
        pass(test_name);
    } else {
        fail(test_name, expected, actual);
    }
}

void test_int(char* test_name, int expected, int actual) {
    if (expected == actual) {
        pass(test_name);
    } else {
        char* expected_string;
        expected_string = (char*)malloc(3);
        
        char* actual_string;
        actual_string = (char*)malloc(3);
        
        sprintf(expected_string, "%i", expected);
        sprintf(actual_string, "%i", actual);
        
        fail(test_name, expected_string, actual_string);
    }
}
