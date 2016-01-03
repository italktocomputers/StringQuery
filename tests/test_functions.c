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

#include "test_functions.h"

void __PREFIX_print_dots(int l) {
    int i=0;
    int n_dots = NUM_SPACES_TO_RIGHT - l;
    
    for (; i<n_dots; i++) {
        printf(".");
    }
}

void __PREFIX_pass(char* test_name) {
    int l = strlen(test_name);
    
    printf("%s", test_name);
    printf("\033[0;32m");
    __PREFIX_print_dots(l);
    printf("\033[0;32m[Pass]\033[0m\n");
}

void __PREFIX_fail(char* test_name, char* expected, char* actual) {
    int l = strlen(test_name);
    
    printf("%s", test_name);
    printf("\033[0;31m");
    __PREFIX_print_dots(l);
    printf("[Fail]\n");
    printf("Expecting %s but got %s\033[0m\n", expected, actual);
}

void __PREFIX_test_string(char* test_name, char* expected, char* actual) {
    if (strcmp(expected, actual) == 0) {
        __PREFIX_pass(test_name);
    } else {
        __PREFIX_fail(test_name, expected, actual);
    }
}

void __PREFIX_test_int(char* test_name, int expected, int actual) {
    if (expected == actual) {
        __PREFIX_pass(test_name);
    } else {
        char* expected_string;
        expected_string = (char*)malloc(3);
        
        char* actual_string;
        actual_string = (char*)malloc(3);
        
        sprintf(expected_string, "%i", expected);
        sprintf(actual_string, "%i", actual);
        
        __PREFIX_fail(test_name, expected_string, actual_string);
    }
}

void __PREFIX_test_resource(char* stringQuery, char* resource) {
    char* resource2 = __PREFIX_get_resource(stringQuery);
    __PREFIX_test_string("Get resource", resource, resource2);
}

void __PREFIX_test_operator(char* stringQuery, char* operator) {
    char* operator2 = __PREFIX_get_operator(stringQuery);
    __PREFIX_test_string("Get operator", operator, operator2);
}

void __PREFIX_test_filter(char* stringQuery, char* filter) {
    char* filter2 = __PREFIX_get_filter(stringQuery);
    __PREFIX_test_string("Get filter", filter, filter2);
}

void __PREFIX_test_conjunctive(char* stringQuery, char* conjunctive) {
    char* conjunctive2 = __PREFIX_get_conjunctive(stringQuery);
    __PREFIX_test_string("Get conjunctive", conjunctive, conjunctive2);
}

void __PREFIX_test_get_filter_type(char* filter, int expected_code) {
    int actual_code = __PREFIX_get_filter_type(filter);
    __PREFIX_test_int("Get filter type", expected_code, actual_code);
}

void __PREFIX_test_validate_resource(char* resource, int expected_code) {
    int actual_code = __PREFIX_validate_resource(resource);
    __PREFIX_test_int("Validate resource", expected_code, actual_code);
}

void __PREFIX_test_validate_operator(char* operator, int expected_code) {
    int actual_code = __PREFIX_validate_operator(operator);
    __PREFIX_test_int("Validate operator", expected_code, actual_code);
}

void __PREFIX_test_validate_conjunctive(char* conj, int expected_code) {
    int actual_code = __PREFIX_validate_conjunctive(conj);
    __PREFIX_test_int("Validate conjunctive", expected_code, actual_code);
}

void __PREFIX_test_validate_var(char* var, int expected_code) {
    int pos;
    int actual_code = __PREFIX_validate_var(var, &pos);
    __PREFIX_test_int("Validate var", expected_code, actual_code);
}
