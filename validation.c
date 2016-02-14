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

#include "validation.h"

int __PREFIX_validate_resource(char value[], int* pos) {
    int i = 0;
    int period = 0;
    int bad_chr = 0;

    *pos = 0;

    while (value[*pos] != '\0') {
        switch (value[*pos]) {
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

        (*pos)++;
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

int __PREFIX_validate_operator(char code[], int* pos) {
    *pos = 0;

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

int __PREFIX_validate_conjunctive(char code[], int* pos) {
    *pos = 0;

    if (strcmp(code, "&") == 0 || strcmp(code, "|") == 0) {
       return NO_ERROR;
    }

    return ERROR_INVALID_CONJUNCTIVE;
}

int __PREFIX_validate_var(char code[], int* pos) {
    *pos = 0;

    if (code[0] != '@') {
        return ERROR_VAR_MUST_START_WITH_IDENTIFIER;
    }

    while (code[*pos] != '\0') {
        switch (code[*pos]) {
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

        (*pos)++;
    }

    return NO_ERROR;
}

int __PREFIX_validate_string(char code[], int* pos) {
    *pos = 0;

    if (code[0] != '\'' && code[0] != '"') {
        return ERROR_INVALID_STRING_NO_START;
    }

    (*pos)++; // Move pass the first quote.

    // I need to find end quote.  I can't just go to end of value and look there
    // since someone could end a string and then add a bunch of spaces.
    while(code[*pos] != '\0') {
        if (code[*pos] == code[0]) {
            if (code[(*pos)-1] != '\\') {
                // This is the end quote.  It is the same quote as the starting
                // quote and is not escaped.
                break;
            }
        }

        (*pos)++;
    }

    // Compare start and end quotes.
    if (code[*pos] != code[0]) {
        return ERROR_INVALID_STRING_NO_END;
    }

    (*pos)++;

    // Now that I know where the string ends, I need to make sure they did not
    // enter anymore characters other than space and line breaks.
    while (code[*pos] != '\0') {
        switch (code[*pos]) {
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
int __PREFIX_validate_list(char code[], int* pos) {
    *pos = 0;

    int length = strlen(code);

    if (code[0] != '(') {
        return ERROR_INVALID_LIST_NO_START;
    }

    if (code[length-1] != ')') {
        return ERROR_INVALID_LIST_NO_END;
    }

    return NO_ERROR;
}

int __PREFIX_validate_int(char code[], int* pos) {
    *pos = 0;

    int length = strlen(code);

    if (length == 0) {
        return ERROR_INVALID_INT;
    }

    while (code[*pos] != '\0') {
        switch (code[*pos]) {
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
                if (*pos == 0)
                    break;
            default:
                return ERROR_INVALID_INT;
        }

        (*pos)++;
    }

    return NO_ERROR;
}

int __PREFIX_validate_double(char code[], int* pos) {
    *pos = 0;

    int dec = 0;
    int length = strlen(code);

    if (length == 0) {
        return ERROR_INVALID_DOUBLE;
    }

    while (code[*pos] != '\0') {
        switch (code[*pos]) {
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
                if (*pos == 0)
                    break;
            default:
                return ERROR_INVALID_DOUBLE;
        }

        (*pos)++;
    }

    if (dec == 0)
        return ERROR_INVALID_DOUBLE;

    return NO_ERROR;
}
