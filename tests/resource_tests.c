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
#include "../parser.h"
#include "resource_tests.h"
#include "test_functions.h"

void get_resource_test1(char* stringQuery) {
    int cursor = 0;
    char* resource = get_resource(stringQuery, &cursor);
    
    test_string("Get resource", "User.FirstName", resource);
    test_int("Check cursor", 15, cursor);
}

void get_resource_test2(char* stringQuery) {
    int cursor = 31;
    char* resource = get_resource(stringQuery, &cursor);
    
    test_string("Get 2nd resource", "User.LastName", resource);
    test_int("Check cursor", 45, cursor);
}

void get_resource_test3(char* stringQuery) {
    int cursor = 62;
    char* resource = get_resource(stringQuery, &cursor);
    
    test_string("Get 3rd resource", "User.Dept", resource);
    test_int("Check cursor", 72, cursor);
}

void run_resource_tests(char* stringQuery) {
    get_resource_test1(stringQuery);
    get_resource_test2(stringQuery);
    get_resource_test3(stringQuery);
}
