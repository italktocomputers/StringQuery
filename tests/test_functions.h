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

#include "../common.h"

#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H
    #define NUM_SPACES_TO_RIGHT 50

    void __PREFIX_print_dots(int l);
    void __PREFIX_pass(char* test_name);
    void __PREFIX_fail(char* test_name, char* expected, char* actual);

    void __PREFIX_test_get_statements(char* test_name, char* stringQuery, char** expected, int length);
    void __PREFIX_test_get_resource(char* test_name, char* stringQuery, char* resource);
    void __PREFIX_test_get_resource_type(char* test_name, char* stringQuery, char* resource);
    void __PREFIX_test_get_operator(char* test_name, char* stringQuery, char* operator);
    void __PREFIX_test_get_filter(char* test_name, char* stringQuery, char* filter);
    void __PREFIX_test_get_conjunctive(char* test_name, char* stringQuery, char* filter);

    void __PREFIX_test_expand_list(char* test_name, char* list, int length, char** expected);

    void __PREFIX_test_validate_resource(char* test_name, char* resource, int expected_code);
    void __PREFIX_test_validate_resource_type(char* test_name, char* resource_type, int expected_code);
    void __PREFIX_test_validate_operator(char* test_name, char* operator, int expected_code);
    void __PREFIX_test_validate_conjunctive(char* test_name, char* conj, int expected_code);
    void __PREFIX_test_validate_var(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_string(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_list(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_int(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_int8_signed(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_int8(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_int16_signed(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_int16(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_int24_signed(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_int24(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_int32_signed(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_int32(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_int64_signed(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_int64(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_double(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_date(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_time(char* test_name, char* var, int expected_code);
    void __PREFIX_test_validate_datetime(char* test_name, char* var, int expected_code);

#endif
