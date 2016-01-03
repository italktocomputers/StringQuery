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

int __PREFIX_validate_resource(char value[]) {
    int i = 0;
    int period = 0;
    int bad_chr = 0;
    
    while (value[i] != '\0') {
        switch (value[i]) {
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
        
        i++;
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

// We try to infer the type.
int __PREFIX_get_filter_type(char code[]) {    
    if (code[0] == '@') {
        return FILTER_TYPE_VAR;
    } else if (code[0] == '(') {
        return FILTER_TYPE_LIST;
    } else if (code[0] == '\'' || code[0] == '"') {
        return FILTER_TYPE_STRING;
    } else {
        // Some sort of number I guess
        int i = 0;
        while (code[i] != '\0') {
            if (code[i] == '.') {
                return FILTER_TYPE_DOUBLE;
            }
            
            i++;
        }
        
        return FILTER_TYPE_INT;
    }
}

int __PREFIX_validate_conjunctive(char code[]) {
    if (strcmp(code, "&") == 0 || strcmp(code, "|") == 0) {
       return NO_ERROR;
    }
    
    return ERROR_INVALID_CONJUNCTIVE;
}

int __PREFIX_validate_var(char code[], int* pos) {
    int i = 0;
    
    if (code[0] != '@') {
        *pos = 0;
        return ERROR_VAR_MUST_START_WITH_IDENTIFIER;
    }
    
    while (code[i] != '\0') {
        switch (code[i]) {
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
                *pos = i;
                return ERROR_INVALID_CHR;
        }
        
        i++;
    }
    
    return NO_ERROR;
}

int __PREFIX_validate_string(char code[]) {
    return 1;
    
    /*
    int i = 0;
    char msg[100];
    char pos = 1;
    
    if (value[0] != '\'' && value[0] != '"') {
        sprintf(msg, "Syntax error.  String must start with a quote but got '%c' instead", value[0]);
        print_error(code, cursor-length, msg, EXIT_INVALID_SYNTAX);
    }
    
    // I need to find end quote.  I can't just go to end of value and look there
    // since someone could end a string and then add a bunch of spaces.
    for (i=1; i<length; i++) {
        if (value[i] == value[0]) {
            if (value[i-1] != '\\') {
                // This is the end quote.  It is the same quote as the starting
                // quote and is not escaped.
                pos = i;
                break;
            }
        }
    }
    
    if (value[pos] != value[0]) {
        sprintf(msg, "Syntax error.  String must have matching ending quote but got '%c' instead", value[length]);
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    }
    
    // Now that I know where the string ends, I need to make sure they did not
    // enter anymore characters other than space and line breaks.
    
    for (i=pos+1; i<length; i++) {
        switch (value[i]) {
            case ' ' :
            case '\n' :
            case '\0' :
                break;
            
            default:
                sprintf(msg, "Syntax error.  Unexpected character", value[length]);
                print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
        }
    }
    */
}

int __PREFIX_validate_list(char code[]) {
    return 1;
    
    /*
    char msg[100];
    
    if (value[0] != '(') {
        sprintf(msg, "Syntax error.  List must start with a '('.  Got '%c' instead", value[0]);
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    }
    
    if (value[length-1] != ')') {
        sprintf(msg, "Syntax error.  List must end with a ')'.  Got '%c' instead", value[length-1]);
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    }
    
    if (strcmp(type, "Int") == 0) {
        validate_list_int(code, cursor, value, length, st);
    } else if(strcmp(type, "Double") == 0) {
        validate_list_double(code, cursor, value, length, st);
    } else if(strcmp(type, "String") == 0) {
        validate_list_string(code, cursor, value, length, st);
    } else {
        exception("Unknown list type: %s", type, EXIT_INVALID_SYNTAX);
    }
    */
}

int __PREFIX_validate_list_string(char code[]) {
    return 1;
    
    /*
    int i = 1;
    int x = 0;
    char newstr[100] = {};
    
    // We need to remove '(' and ')' characters
    for (; i<length-1; i++) {
        if (value[i] == ' ')
            continue; // skip spaces
            
        newstr[x++] = value[i];
    }
    
    char* token;
    token = strtok(newstr, ",");
    
    while (token != NULL) {
        int l = strlen(token);
        validate_string(code, cursor, token, l, st);
        token = strtok (NULL, ",");
    }
    */
}

int __PREFIX_validate_list_int(char code[]) {
    return 1;
    
    /*
    int i = 1;
    int x = 0;
    char newstr[100] = {};
    
    // We need to remove '(' and ')' characters
    for (; i<length-1; i++) {
        if (value[i] == ' ')
            continue; // skip spaces
            
        newstr[x++] = value[i];
    }
    
    char* token;
    token = strtok(newstr, ",");
    
    while (token != NULL) {
        int l = strlen(token);
        validate_int(code, cursor, token, l, st);
        token = strtok (NULL, ",");
    }
    */
}

int __PREFIX_validate_list_double(char code[]) {
    return 1;
    
    /*
    int i = 1;
    int x = 0;
    char newstr[100] = {};
    
    // We need to remove '(' and ')' characters
    for (; i<length-1; i++) {
        if (value[i] == ' ')
            continue; // skip spaces
            
        newstr[x++] = value[i];
    }
    
    char* token;
    token = strtok(newstr, ",");
    
    while (token != NULL) {
        int l = strlen(token);
        validate_double(code, cursor, token, l, st);
        token = strtok (NULL, ",");
    }
    */
}

int __PREFIX_validate_int(char code[]) {
    return 1;
    
    /*
    int i = 0;
    int error = 0;
    char msg[100];
    
    if (length == 0) {
        sprintf(msg, "Syntax error.  Not a integer");
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    }
    
    for (; i<length; i++) {
        switch (value[i]) {
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
                if (i == 0)
                    break;
            default:
                error = 1;
        }
        
        if (error == 1)
            break;
    }
    
    if (error == 1) {
        if (strlen(value) >= 50) {
            sprintf(msg, "Syntax error.  Not a integer");
        } else {
            sprintf(msg, "Syntax error.  Not a integer: '%s'", value);
        }
        
        print_error(code, cursor-length+i, msg, EXIT_INVALID_SYNTAX);
    }
    */
}

int __PREFIX_validate_double(char code[]) {
    return 1;
    
    /*
    int i = 0;
    int dec = 0;
    int error = 0;
    char msg[100];
    
    if (length == 0) {
        sprintf(msg, "Syntax error.  Not a double");
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    }
    
    for (i=0; i<length; i++) { 
        switch (value[i]) {
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
                    error = 2;
                break;
            case '-' :
                // '-' allowed at beginning
                if (i == 0)
                    break;
            default:
                error = 1;
        }
        
        if (error != 0)
            break;
    }
    
    if (error == 1) {
        if (strlen(value) >= 50) {
            sprintf(msg, "Syntax error.  Not a decimal");
        } else {
            sprintf(msg, "Syntax error.  Not a decimal: '%s'", value);
        }
        
        print_error(code, cursor-length+i, msg, EXIT_INVALID_SYNTAX);
    } else if (error == 2) {
        if (strlen(value) >= 50) {
            sprintf(msg, "Syntax error.  Too many decimals");
        } else {
            sprintf(msg, "Syntax error.  Too many decimals '%s'", value);
        }
        
        print_error(code, cursor-length+i, msg, EXIT_INVALID_SYNTAX);
    }
    */
}
