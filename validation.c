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
#include "validation.h"
#include "library.h"

//
// A resource must be in the following format: String.String
//
int __PREFIX_validate_resource(char value[]) {
    int period = 0;
    int bad_chr = 0;
    int pos = 0;

    while (value[pos] != '\0') {
        switch (value[pos]) {
            case '&' :
            case '*' :
            case '(' :
            case ')' :
            case '+' :
            case '-' :
            case '\'' :
            case ':' :
            case '"' :
            case ',' :
            case '?' :
            case '\\' :
            case '@' :
            case '>' :
            case '<' :
            case '=' :
            case '!' :
                // They cannot use any of these characters in their Resources
                // name.
                bad_chr = 1;
                break;
            case '.' :
                period++;
                break;
        }

        pos++;
    }

    int x = strlen(value);

    if (bad_chr == 1) {
        return ERROR_INVALID_CHR;
    } else if (x == 0) {
        return ERROR_INVALID_RESOURCE;
    } else if (period > 1) {
        return ERROR_INVALID_RESOURCE;
    } else if (period == 0) {
        return ERROR_INVALID_RESOURCE;
    }

    return NO_ERROR;
}

//
// A resource type can be one of the following:
// - String
// - Int8
// - uInt8
// - Int16
// - uInt16
// - Int24
// - uInt24
// - Int32
// - uInt32
// - Int64
// - uInt64
// - Double
// - Date
// - Time
// - DateTime
// - NULL
// - @
//
int __PREFIX_validate_resource_type(char code[]) {
    if (strcmp(code, "String") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "Int8") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "uInt8") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "Int16") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "uInt16") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "Int24") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "uInt24") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "Int32") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "uInt32") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "Int64") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "uInt64") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "Double") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "Date") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "Time") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "DateTime") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "@") == 0) {
        return NO_ERROR;
    } else {
        return ERROR_INVALID_RESOURCE_TYPE;
    }
}

//
// An operator can be one of the following values:
//     !=
//     >
//     >=
//     <
//     <=
//
int __PREFIX_validate_operator(char code[]) {
    if (strcmp(code, "=") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "!=") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, ">") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, ">=") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "<") == 0) {
        return NO_ERROR;
    } else if (strcmp(code, "<=") == 0) {
        return NO_ERROR;
    } else {
        return ERROR_INVALID_OPERATOR;
    }
}

//
// A conjunctive can be either a '&' or a '|'
//
int __PREFIX_validate_conjunctive(char code[]) {
    if (strcmp(code, "&") == 0 || strcmp(code, "|") == 0) {
       return NO_ERROR;
    }

    return ERROR_INVALID_CONJUNCTIVE;
}

//
// A variable must start with a '@' and is limited with what characters can be
// used in its name.
//
int __PREFIX_validate_var(char code[]) {
    int pos = 0;

    if (code[0] != '@') {
        return ERROR_VAR_MUST_START_WITH_IDENTIFIER;
    }

    while (code[pos] != '\0') {
        switch (code[pos]) {
            case '&' :
            case '*' :
            case '(' :
            case ')' :
            case '+' :
            case '-' :
            case '\'' :
            case ':' :
            case '"' :
            case ',' :
            case '?' :
            case '\\' :
            case '>' :
            case '<' :
            case '=' :
            case '!' :
            case '.' :
                // They cannot use any of these characters in their variable
                // name.
                return ERROR_INVALID_CHR;
        }

        pos++;
    }

    return NO_ERROR;
}

//
// A string must be contained in either single or double quotes.
//
int __PREFIX_validate_string(char code[]) {
    int pos = 0;

    if (code[0] != '\'' && code[0] != '"') {
        return ERROR_INVALID_STRING_NO_START;
    }

    pos++; // Move pass the first quote.

    // I need to find end quote.  I can't just go to end of value and look there
    // since someone could end a string and then add a bunch of spaces.
    while(code[pos] != '\0') {
        if (code[pos] == code[0]) {
            if (code[pos-1] != '\\') {
                // This is the end quote.  It is the same quote as the starting
                // quote and is not escaped.
                break;
            }
        }

        pos++;
    }

    // Compare start and end quotes.
    if (code[pos] != code[0]) {
        return ERROR_INVALID_STRING_NO_END;
    }

    pos++;

    // Now that I know where the string ends, I need to make sure they did not
    // enter anymore characters other than space and line breaks.
    while (code[pos] != '\0') {
        switch (code[pos]) {
            case ' ' :
            case '\n' :
            case '\0' :
                break;

            default:
                return ERROR_INVALID_STRING;
        }
    }

    return NO_ERROR;
}

//
// A list can be made up of different types.  Because of this, we don't need to
// worry about the validity of each item, only that the syntax is correct:
//
// (item1[,item2])
//
int __PREFIX_validate_list(char code[]) {
    int pos = 0;

    int length = strlen(code);

    if (code[0] != '(') {
        return ERROR_INVALID_LIST_NO_START;
    }

    if (code[length-1] != ')') {
        return ERROR_INVALID_LIST_NO_END;
    }

    return NO_ERROR;
}

//
// Int should be any number that does NOT contain a decimal point.  The purpose
// of this function is to validate that there is a valid Integer here, NOT
// validate the correct size of the Int.
//
int __PREFIX_validate_int(char code[]) {
    int pos = 0;
    int length = strlen(code);

    if (length == 0) {
        return ERROR_INVALID_INT;
    }

    while (code[pos] != '\0') {
        switch (code[pos]) {
            case '0' :
            case '1' :
            case '2' :
            case '3' :
            case '4' :
            case '5' :
            case '6' :
            case '7' :
            case '8' :
            case '9' :
                break;
            case '-' :
                // '-' allowed at beginning
                if (pos == 0)
                    break;
            default:
                return ERROR_INVALID_INT;
        }

        pos++;
    }

    return NO_ERROR;
}

//
// Acceptable range of values: -128 to 127
//
int __PREFIX_validate_int8_signed(char code[]) {
     char *ptr;
     long int number = strtol(code, &ptr, 10);

    if (number >= -128 && number <= 127)
        return NO_ERROR;

    return ERROR_INVALID_INT;
}

//
// Acceptable range of values: 0 to 255
//
int __PREFIX_validate_int8(char code[]) {
    char *ptr;
    long int number = strtol(code, &ptr, 10);

    if (number >= 0 && number <= 255)
        return NO_ERROR;

    return ERROR_INVALID_INT;
}

//
// Acceptable range of values: -32768 to 32767
//
int __PREFIX_validate_int16_signed(char code[]) {
    char *ptr;
    long int number = strtol(code, &ptr, 10);

    if (number >= -32768 && number <= 32767)
        return NO_ERROR;

    return ERROR_INVALID_INT;
}

//
// Acceptable range of values: 0 to 65535
//
int __PREFIX_validate_int16(char code[]) {
    char *ptr;
    long int number = strtol(code, &ptr, 10);

    if (number >= 0 && number <= 65535)
        return NO_ERROR;

    return ERROR_INVALID_INT;
}

//
// Acceptable range of values: -8388608 to 8388607
//
int __PREFIX_validate_int24_signed(char code[]) {
    char *ptr;
    long int number = strtol(code, &ptr, 10);

    if (number >= -8388608 && number <= 8388607)
        return NO_ERROR;

    return ERROR_INVALID_INT;
}

//
// Acceptable range of values: 0 to 16777215
//
int __PREFIX_validate_int24(char code[]) {
    char *ptr;
    long int number = strtol(code, &ptr, 10);

    if (number >= 0 && number <= 16777215)
        return NO_ERROR;

    return ERROR_INVALID_INT;
}

//
// Acceptable range of values: -2147483648 to 2147483647
//
int __PREFIX_validate_int32_signed(char code[]) {
    char *ptr;
    long int number = strtol(code, &ptr, 10);

    if (number >= -2147483648 && number <= 2147483647)
        return NO_ERROR;

    return ERROR_INVALID_INT;
}

//
// Acceptable range of values: 0 to 4294967295
//
int __PREFIX_validate_int32(char code[]) {
    char *ptr;
    long int number = strtol(code, &ptr, 10);

    if (number >= 0 && number <= 4294967295)
        return NO_ERROR;

    return ERROR_INVALID_INT;
}

//
// Acceptable range of values: -9223372036854775808 to 9223372036854775807
//
int __PREFIX_validate_int64_signed(char code[]) {
    char *ptr;
    long double number = strtold(code, &ptr);

    if (number >= -9223372036854775808.0L && number <= 9223372036854775807.0L)
        return NO_ERROR;

    return ERROR_INVALID_INT;
}

//
// Acceptable range of values: 0 to 18446744073709551615
//
int __PREFIX_validate_int64(char code[]) {
    char *ptr;
    long double number = strtold(code, &ptr);

    if (number >= 0.0 && number <= 18446744073709551615.0L)
        return NO_ERROR;

    return ERROR_INVALID_INT;
}

//
// Double should be any number that contains a decimal point.
//
int __PREFIX_validate_double(char code[]) {
    int pos = 0;
    int dec = 0;
    int length = strlen(code);

    if (length == 0) {
        return ERROR_INVALID_DOUBLE;
    }

    while (code[pos] != '\0') {
        switch (code[pos]) {
            case '0' :
            case '1' :
            case '2' :
            case '3' :
            case '4' :
            case '5' :
            case '6' :
            case '7' :
            case '8' :
            case '9' :
                break;
            case '.' :
                dec++;
                if (dec > 1)
                    return ERROR_INVALID_DOUBLE;
                break;
            case '-' :
                // '-' allowed at beginning
                if (pos == 0)
                    break;
            default:
                return ERROR_INVALID_DOUBLE;
        }

        pos++;
    }

    if (dec == 0)
        return ERROR_INVALID_DOUBLE;

    return NO_ERROR;
}

//
// Date should be in the following format: YYYY-MM-DD
//
int __PREFIX_validate_date(char code[]) {
    if (strlen(code) != 10)
        return ERROR_INVALID_DATE;

    int pos = 0;
    int i = 0;
    char buffer[4];

    char year[5];
    char month[3];
    char day[3];

    char* yearJunk;
    char* monthJunk;
    char* dayJunk;

    do {
        buffer[i++] = code[pos];

        if (pos == 3) {
            buffer[i] = '\0';
            strcpy(year, buffer);
            i = 0;
            pos++; // skip '-'
        } else if (pos == 6) {
            buffer[i] = '\0';
            strcpy(month, buffer);
            i = 0 ;
            pos++; // skip '-'
        } else if (pos == 9) {
            buffer[i] = '\0';
            strcpy(day, buffer);
        }
    } while (code[pos++] != '\0');

    int iyear = strtol(year, &yearJunk, 10);
    int imonth = strtol(month, &monthJunk, 10);
    int iday = strtol(day, &dayJunk, 10);

    if (strlen(yearJunk) > 0) {
        return ERROR_INVALID_YEAR;
    } else if (strlen(monthJunk) > 0) {
        return ERROR_INVALID_MONTH;
    } else if (strlen(dayJunk) > 0) {
        return ERROR_INVALID_DAY;
    } else if (iyear < 0 || iyear > 9999) {
        return ERROR_INVALID_YEAR;
    } else if (imonth < 1 || imonth > 12) {
        return ERROR_INVALID_MONTH;
    } else if (iday < 1 || iday > 31) {
        return ERROR_INVALID_DAY;
    }

    return NO_ERROR;
}

//
// Time should be in the following format: HH:MM:SS
//
int __PREFIX_validate_time(char code[]) {
    if (strlen(code) != 8)
        return ERROR_INVALID_TIME;

    int pos = 0;
    int i = 0;
    char buffer[4];

    char hour[3];
    char minute[3];
    char second[3];

    char* hourJunk;
    char* minuteJunk;
    char* secondJunk;

    do {
        buffer[i++] = code[pos];

        if (pos == 1) {
            buffer[i] = '\0';
            strcpy(hour, buffer);
            i = 0;
            pos++; // skip ':'
        } else if (pos == 4) {
            buffer[i] = '\0';
            strcpy(minute, buffer);
            i = 0 ;
            pos++; // skip ':'
        } else if (pos == 7) {
            buffer[i] = '\0';
            strcpy(second, buffer);
        }
    } while (code[pos++] != '\0');

    int ihour = strtol(hour, &hourJunk, 10);
    int iminute = strtol(minute, &minuteJunk, 10);
    int isecond = strtol(second, &secondJunk, 10);

    if (strlen(hourJunk) > 0) {
        return ERROR_INVALID_HOUR;
    } else if (strlen(minuteJunk) > 0) {
        return ERROR_INVALID_MINUTE;
    } else if (strlen(secondJunk) > 0) {
        return ERROR_INVALID_SECOND;
    } else if (ihour > 23) {
        return ERROR_INVALID_HOUR;
    } else if (iminute > 60) {
        return ERROR_INVALID_MINUTE;
    } else if (isecond > 60) {
        return ERROR_INVALID_SECOND;
    }

    return NO_ERROR;
}

//
// DateTime should be in the following format: YYYY-MM-DD HH:MM:SS
//
int __PREFIX_validate_datetime(char code[]) {
    char* date = (char*)malloc(11);
    char* time = (char*)malloc(9);

    __PREFIX_substr(code, 0, 9, date, 11);
    __PREFIX_substr(code, 11, 18, time, 9);

    int dateVal = __PREFIX_validate_date(date);

    if (dateVal != NO_ERROR)
        return dateVal;

    int timeVal = __PREFIX_validate_time(time);

    if (timeVal != NO_ERROR)
        return timeVal;

    return NO_ERROR;
}
