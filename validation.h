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

#include "common.h"

#ifndef VALIDATION_H
#define VALIDATION_H

    int __PREFIX_validate_resource(char[]);
    int __PREFIX_validate_resource_type(char[]);
    int __PREFIX_validate_operator(char[]);
    int __PREFIX_validate_conjunctive(char[]);
    int __PREFIX_validate_var(char code[]);
    int __PREFIX_validate_string(char[]);
    int __PREFIX_validate_int(char[]);
    int __PREFIX_validate_int8(char[]);
    int __PREFIX_validate_int8_signed(char[]);
    int __PREFIX_validate_int16(char[]);
    int __PREFIX_validate_int16_signed(char[]);
    int __PREFIX_validate_int24(char[]);
    int __PREFIX_validate_int24_signed(char[]);
    int __PREFIX_validate_int32(char[]);
    int __PREFIX_validate_int32_signed(char[]);
    int __PREFIX_validate_int64(char[]);
    int __PREFIX_validate_int64_signed(char[]);
    int __PREFIX_validate_double(char[]);
    int __PREFIX_validate_list(char[]);
    int __PREFIX_validate_date(char[]);
    int __PREFIX_validate_time(char[]);
    int __PREFIX_validate_datetime(char[]);

#endif
