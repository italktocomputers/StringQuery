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
char* stringQuery1 = "User.FirstName:String='Andrew'&User.LastName:String='Schools'&User.Dept:String=('Engineering','Math')";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9
char* stringQuery2 = "User.FirstName:String='Andrew'|User.FirstName:String='Laura'";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9
char* stringQuery3 = "User.FirstName:String='Andrew'";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9
char* stringQuery4 = "User.FirstName:String!='Andrew'";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9
char* stringQuery5 = "User.Age:Int8>5";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9
char* stringQuery6 = "User.Age:Int8>=5";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9
char* stringQuery7 = "User.Age:Double<5.5";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9
char* stringQuery8 = "User.Age:Int8<=5";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

//////////////////////0         1         2         3         4         5         6         7         8         9
char* stringQuery9 = "User.Dept:String=('Engineering','Math')";
//////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

///////////////////////0         1         2         3         4         5         6         7         8         9
char* stringQuery10 = "User.Dept:String!=('Engineering','Math')";
///////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

///////////////////////0         1         2         3         4         5         6         7         8         9
char* stringQuery11 = "User.Dept:String!=('Engineering','Math')&";
///////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

///////////////////////0         1         2         3         4         5         6         7         8         9
char* stringQuery12 = "User.Dept:String!=('Engineering','Math')|";
///////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

///////////////////////0         1         2         3         4         5         6         7         8         9
char* stringQuery13 = "User.CreatedDate:DateTime=2016-01-02 03:04:05";
///////////////////////01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890


int main(int argc, const char* argv[]) {
    __PREFIX_test_get_resource("Test get_resource valid", stringQuery3, "User.FirstName");

    __PREFIX_test_get_resource_type("Test get_resource_type", stringQuery12, "String");

    __PREFIX_test_get_operator("Test get_operator =", stringQuery3, "=");
    __PREFIX_test_get_operator("Test get_operator !=", stringQuery4, "!=");
    __PREFIX_test_get_operator("Test get_operator >", stringQuery5, ">");
    __PREFIX_test_get_operator("Test get_operator >=", stringQuery6, ">=");
    __PREFIX_test_get_operator("Test get_operator <", stringQuery7, "<");
    __PREFIX_test_get_operator("Test get_operator <=", stringQuery8, "<=");

    __PREFIX_test_get_filter("Test get_filter string", stringQuery3, "'Andrew'");
    __PREFIX_test_get_filter("Test get_filter int", stringQuery6, "5");
    __PREFIX_test_get_filter("Test get_filter double", stringQuery7, "5.5");
    __PREFIX_test_get_filter("Test get_filter list", stringQuery9, "('Engineering','Math')");

    __PREFIX_test_get_conjunctive("Test get_conjunctive &", stringQuery11, "&");
    __PREFIX_test_get_conjunctive("Test get_conjunctive |", stringQuery12, "|");

    __PREFIX_test_validate_resource("Test validate_resource ok", "User.FirstName", NO_ERROR);
    __PREFIX_test_validate_resource("Test validate_resource too many periods", "User..FirstName", ERROR_INVALID_RESOURCE);
    __PREFIX_test_validate_resource("Test validate_resource no period", "FirstName", ERROR_INVALID_RESOURCE);
    __PREFIX_test_validate_resource("Test validate_resource empty", "", ERROR_INVALID_RESOURCE);
    __PREFIX_test_validate_resource("Test validate_resource bad chr *", "FirstName*", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr (", "FirstName(", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr )", "FirstName)", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr +", "FirstName+", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr -", "FirstName-", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr '", "FirstName'", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr :", "FirstName:", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr \"", "FirstName\"", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr ,", "FirstName,", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr ?", "FirstName?", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr \\", "FirstName\\", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr @", "FirstName@", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr >", "FirstName>", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr <", "FirstName<", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr =", "FirstName=", ERROR_INVALID_CHR);
    __PREFIX_test_validate_resource("Test validate_resource bad chr !", "FirstName!", ERROR_INVALID_CHR);

    __PREFIX_test_validate_operator("Test validate_operator =", "=", NO_ERROR);
    __PREFIX_test_validate_operator("Test validate_operator !=", "!=", NO_ERROR);
    __PREFIX_test_validate_operator("Test validate_operator >", ">", NO_ERROR);
    __PREFIX_test_validate_operator("Test validate_operator >=", ">=", NO_ERROR);
    __PREFIX_test_validate_operator("Test validate_operator <", "<", NO_ERROR);
    __PREFIX_test_validate_operator("Test validate_operator <=", "<=", NO_ERROR);
    __PREFIX_test_validate_operator("Test validate_operator %", "%", ERROR_INVALID_OPERATOR);

    __PREFIX_test_validate_conjunctive("Test validate_conjunctive ok &", "&", NO_ERROR);
    __PREFIX_test_validate_conjunctive("Test validate_conjunctive ok |", "|", NO_ERROR);
    __PREFIX_test_validate_conjunctive("Test validate_conjunctive bad %", "%", ERROR_INVALID_CONJUNCTIVE);

    __PREFIX_test_validate_var("Test validate_var ok", "@FirstName", NO_ERROR);
    __PREFIX_test_validate_var("Test validate_var no identifier", "FirstName", ERROR_VAR_MUST_START_WITH_IDENTIFIER);
    __PREFIX_test_validate_var("Test validate_var bad chr *", "@FirstName*", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr (", "@FirstName(1", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr )", "@FirstName)b", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr +", "@Name+", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr -", "@FirstName-", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr '", "@FirstName'", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr :", "@FirstName:", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr \"", "@FirstName\"", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr ,", "@FirstName,", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr ?", "@FirstName?", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr \\", "@FirstName\\", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr >", "@FirstName>", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr <", "@FirstName<", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr =", "@FirstName=", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr !", "@FirstName!", ERROR_INVALID_CHR);
    __PREFIX_test_validate_var("Test validate_var bad chr .", "@FirstName.", ERROR_INVALID_CHR);

    __PREFIX_test_validate_string("Test validate_string valid string", "'Andrew'", NO_ERROR);
    __PREFIX_test_validate_string("Test validate_string with escaped single quote", "'Andrew\\'s'", NO_ERROR);
    __PREFIX_test_validate_string("Test validate_string with escaped double quote", "\"Andrew\"", NO_ERROR);
    __PREFIX_test_validate_string("Test validate_string with special characters", "'!@#$%^&*()_+-={}[]|\\<>?,./:\";'", NO_ERROR);
    __PREFIX_test_validate_string("Test validate_string missing start quote", "Andrew'", ERROR_INVALID_STRING_NO_START);
    __PREFIX_test_validate_string("Test validate_string missing end quote", "'Andrew", ERROR_INVALID_STRING_NO_END);
    __PREFIX_test_validate_string("Test validate_string invalid string", "'Andrew'Schools", ERROR_INVALID_STRING);

    __PREFIX_test_validate_list("Test validate_list valid", "(a,b)", NO_ERROR);
    __PREFIX_test_validate_list("Test validate_list invalid", "a", ERROR_INVALID_LIST_NO_START);
    __PREFIX_test_validate_list("Test validate_list invalid", "(a", ERROR_INVALID_LIST_NO_END);

    __PREFIX_test_validate_int("Test validate_int valid", "1", NO_ERROR);
    __PREFIX_test_validate_int("Test validate_int valid", "-1", NO_ERROR);
    __PREFIX_test_validate_int("Test validate_int invalid", "a", ERROR_INVALID_INT);
    __PREFIX_test_validate_int("Test validate_int invalid", "1.0", ERROR_INVALID_INT);

    __PREFIX_test_validate_int8_signed("Test validate_int8_signed valid", "-128", NO_ERROR);
    __PREFIX_test_validate_int8_signed("Test validate_int8_signed valid", "127", NO_ERROR);
    __PREFIX_test_validate_int8_signed("Test validate_int8_signed invalid", "-129", ERROR_INVALID_INT);
    __PREFIX_test_validate_int8_signed("Test validate_int8_signed invalid", "128", ERROR_INVALID_INT);

    __PREFIX_test_validate_int8("Test validate_int8 valid", "0", NO_ERROR);
    __PREFIX_test_validate_int8("Test validate_int8 valid", "255", NO_ERROR);
    __PREFIX_test_validate_int8("Test validate_int8 invalid", "-1", ERROR_INVALID_INT);
    __PREFIX_test_validate_int8("Test validate_int8 invalid", "256", ERROR_INVALID_INT);

    __PREFIX_test_validate_int16_signed("Test validate_int16_signed valid", "-32768", NO_ERROR);
    __PREFIX_test_validate_int16_signed("Test validate_int16_signed valid", "32767", NO_ERROR);
    __PREFIX_test_validate_int16_signed("Test validate_int16_signed invalid", "-32769", ERROR_INVALID_INT);
    __PREFIX_test_validate_int16_signed("Test validate_int16_signed invalid", "32768", ERROR_INVALID_INT);

    __PREFIX_test_validate_int16("Test validate_int16 valid", "0", NO_ERROR);
    __PREFIX_test_validate_int16("Test validate_int16 valid", "65535", NO_ERROR);
    __PREFIX_test_validate_int16("Test validate_int16 invalid", "-1", ERROR_INVALID_INT);
    __PREFIX_test_validate_int16("Test validate_int16 invalid", "65536", ERROR_INVALID_INT);

    __PREFIX_test_validate_int24_signed("Test validate_int24_signed valid", "-8388608", NO_ERROR);
    __PREFIX_test_validate_int24_signed("Test validate_int24_signed valid", "8388607", NO_ERROR);
    __PREFIX_test_validate_int24_signed("Test validate_int24_signed invalid", "-8388609", ERROR_INVALID_INT);
    __PREFIX_test_validate_int24_signed("Test validate_int24_signed invalid", "8388608", ERROR_INVALID_INT);

    __PREFIX_test_validate_int24("Test validate_int24 valid", "0", NO_ERROR);
    __PREFIX_test_validate_int24("Test validate_int24 valid", "16777215", NO_ERROR);
    __PREFIX_test_validate_int24("Test validate_int24 invalid", "-1", ERROR_INVALID_INT);
    __PREFIX_test_validate_int24("Test validate_int24 invalid", "16777216", ERROR_INVALID_INT);

    __PREFIX_test_validate_int32_signed("Test validate_int32_signed valid", "-2147483648", NO_ERROR);
    __PREFIX_test_validate_int32_signed("Test validate_int32_signed valid", "2147483647", NO_ERROR);
    __PREFIX_test_validate_int32_signed("Test validate_int32_signed invalid", "-2147483649", ERROR_INVALID_INT);
    __PREFIX_test_validate_int32_signed("Test validate_int32_signed invalid", "2147483648", ERROR_INVALID_INT);

    __PREFIX_test_validate_int32("Test validate_int32 valid", "0", NO_ERROR);
    __PREFIX_test_validate_int32("Test validate_int32 valid", "4294967295", NO_ERROR);
    __PREFIX_test_validate_int32("Test validate_int32 invalid", "-1", ERROR_INVALID_INT);
    __PREFIX_test_validate_int32("Test validate_int32 invalid", "4294967296", ERROR_INVALID_INT);

    __PREFIX_test_validate_int64_signed("Test validate_int64_signed valid", "-9223372036854775808", NO_ERROR);
    __PREFIX_test_validate_int64_signed("Test validate_int64_signed valid", "9223372036854775807", NO_ERROR);

    __PREFIX_test_validate_int64_signed("Test validate_int64_signed invalid", "-9223372036854775809", ERROR_INVALID_INT);
    __PREFIX_test_validate_int64_signed("Test validate_int64_signed invalid", "9223372036854775808", ERROR_INVALID_INT);

    __PREFIX_test_validate_int64("Test validate_int64 valid", "0", NO_ERROR);
    __PREFIX_test_validate_int64("Test validate_int64 valid", "18446744073709551615", NO_ERROR);
    __PREFIX_test_validate_int64("Test validate_int64 invalid", "-1", ERROR_INVALID_INT);
    __PREFIX_test_validate_int64("Test validate_int64 invalid", "18446744073709551616", ERROR_INVALID_INT);

    __PREFIX_test_validate_double("Test validate_double valid", "1.0", NO_ERROR);
    __PREFIX_test_validate_double("Test validate_double valid", "-1.0", NO_ERROR);
    __PREFIX_test_validate_double("Test validate_double invalid", "a", ERROR_INVALID_DOUBLE);
    __PREFIX_test_validate_double("Test validate_double invalid", "1", ERROR_INVALID_DOUBLE);

    __PREFIX_test_validate_datetime("Test validate_datetime valid", "2016-02-15 01:02:03", NO_ERROR);
    __PREFIX_test_validate_datetime("Test validate_datetime invalid month", "2016-13-15 01:02:03", ERROR_INVALID_MONTH);
    __PREFIX_test_validate_datetime("Test validate_datetime invalid day", "2016-02-32 01:02:03", ERROR_INVALID_DAY);
    __PREFIX_test_validate_datetime("Test validate_datetime invalid hour", "2016-02-15 25:02:03", ERROR_INVALID_HOUR);
    __PREFIX_test_validate_datetime("Test validate_datetime invalid minute", "2016-02-15 01:61:03", ERROR_INVALID_MINUTE);
    __PREFIX_test_validate_datetime("Test validate_datetime invalid second", "2016-02-15 01:02:61", ERROR_INVALID_SECOND);

}
