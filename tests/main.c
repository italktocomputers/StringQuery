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
#include <assert.h>  
#include "../common.h"
#include "test_functions.h"

//////////////////////0         1         2         3         4         5         6         7         8         9         
char* stringQuery1 = "User.FirstName='Andrew'&User.LastName='Schools'&User.Dept=('Engineering','Math')";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9         
char* stringQuery2 = "User.FirstName='Andrew'|User.FirstName='Laura'";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9         
char* stringQuery3 = "User.FirstName='Andrew'";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9         
char* stringQuery4 = "User.FirstName!='Andrew'";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9         
char* stringQuery5 = "User.Age>5";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9         
char* stringQuery6 = "User.Age>=5";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9         
char* stringQuery7 = "User.Age<5.5";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9         
char* stringQuery8 = "User.Age<=5";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9         
char* stringQuery9 = "User.Dept=('Engineering','Math')";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

///////////////////////0         1         2         3         4         5         6         7         8         9         
char* stringQuery10 = "User.Dept!=('Engineering','Math')";
///////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

int main(int argc, const char* argv[]) {
    __PREFIX_test_resource(stringQuery3, "User.FirstName");
    
    __PREFIX_test_operator(stringQuery3, "=");
    __PREFIX_test_operator(stringQuery4, "!=");
    __PREFIX_test_operator(stringQuery5, ">");
    __PREFIX_test_operator(stringQuery6, ">=");
    __PREFIX_test_operator(stringQuery7, "<");
    __PREFIX_test_operator(stringQuery8, "<=");
    
    __PREFIX_test_filter(stringQuery3, "'Andrew'");
    __PREFIX_test_filter(stringQuery6, "5");
    __PREFIX_test_filter(stringQuery7, "5.5");
    __PREFIX_test_filter(stringQuery9, "('Engineering','Math')");
}
