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
#include "../common.h"
#include "../validation.h"
#include "../extraction.h"

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

    exit(0);
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

void __PREFIX_test_char_pointer_array(char* test_name, char** expected, int length, char** actual) {
    int i = 0;

    for (; i<length; i++) {
        if (strcmp(expected[i], actual[i]) != 0) {
            __PREFIX_fail(test_name, expected[i], actual[i]);
        }
    }

    __PREFIX_pass(test_name);
}

void __PREFIX_test_get_statements(char* test_name, char* stringQuery, char** expected, int length) {
    char** actual = __PREFIX_get_statements(stringQuery);
    __PREFIX_test_char_pointer_array(test_name, expected, length, actual);
}

void __PREFIX_test_get_resource(char* test_name, char* stringQuery, char* resource) {
    char* resource2 = __PREFIX_get_resource(stringQuery);
    __PREFIX_test_string(test_name, resource, resource2);
}

void __PREFIX_test_get_resource_type(char* test_name, char* stringQuery, char* resource) {
    char* resource2 = __PREFIX_get_resource_type(stringQuery);
    __PREFIX_test_string(test_name, resource, resource2);
}

void __PREFIX_test_get_operator(char* test_name, char* stringQuery, char* operator) {
    char* operator2 = __PREFIX_get_operator(stringQuery);
    __PREFIX_test_string(test_name, operator, operator2);
}

void __PREFIX_test_get_filter(char* test_name, char* stringQuery, char* filter) {
    char* filter2 = __PREFIX_get_filter(stringQuery);
    __PREFIX_test_string(test_name, filter, filter2);
}

void __PREFIX_test_get_conjunctive(char* test_name, char* stringQuery, char* conjunctive) {
    char* conjunctive2 = __PREFIX_get_conjunctive(stringQuery);
    __PREFIX_test_string(test_name, conjunctive, conjunctive2);
}

void __PREFIX_test_validate_resource(char* test_name, char* resource, int expected_code) {
    int actual_code = __PREFIX_validate_resource(resource);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_operator(char* test_name, char* operator, int expected_code) {
    int actual_code = __PREFIX_validate_operator(operator);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_conjunctive(char* test_name, char* conj, int expected_code) {
    int actual_code = __PREFIX_validate_conjunctive(conj);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_var(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_var(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_string(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_string(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_list(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_list(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_int(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_int(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_int8_signed(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_int8_signed(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_int8(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_int8(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_int16_signed(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_int16_signed(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_int16(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_int16(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_int24_signed(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_int24_signed(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_int24(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_int24(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_int32_signed(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_int32_signed(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_int32(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_int32(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_int64_signed(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_int64_signed(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_int64(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_int64(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_double(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_double(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_date(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_date(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_time(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_time(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}

void __PREFIX_test_validate_datetime(char* test_name, char* var, int expected_code) {
    int actual_code = __PREFIX_validate_datetime(var);
    __PREFIX_test_int(test_name, expected_code, actual_code);
}
