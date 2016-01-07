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
#include "../common.h"
#include "../validation.h"
#include "../extraction.h"

#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H
    #define NUM_SPACES_TO_RIGHT 50
    
    void __PREFIX_print_dots(int l);
    void __PREFIX_pass(char* test_name);
    void __PREFIX_fail(char* test_name, char* expected, char* actual);
    
    void __PREFIX_test_string(char* test_name, char* expected, char* actual);
    void __PREFIX_test_int(char* test_name, int expected, int actual);
    void __PREFIX_test_resource(char* stringQuery, char* resource);
    void __PREFIX_test_operator(char* stringQuery, char* operator);
    void __PREFIX_test_filter(char* stringQuery, char* filter);
    void __PREFIX_test_conjunctive(char* stringQuery, char* filter);
    
    void __PREFIX_test_get_filter_type(char* filter, int expected_code);
    
    void __PREFIX_test_validate_resource(char* resource, int expected_code);
    void __PREFIX_test_validate_operator(char* operator, int expected_code);
    void __PREFIX_test_validate_conjunctive(char* conj, int expected_code);
    void __PREFIX_test_validate_var(char* var, int cursor, int expected_code);
    
#endif
