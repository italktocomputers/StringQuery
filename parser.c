#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"
#include "parser.h"

char* get_resource(char code[], int* cursor) {
    int start = *cursor;
    int tmp_cursor = *cursor;
    
    // Find the ':' delimiter
    while (code[tmp_cursor] != '\0') {
        if (code[tmp_cursor] == ':') {
            tmp_cursor--;
            break;
        } else if (code[tmp_cursor] == '=') {
            // They forgot to add a type. Even though this is not a valid 
            // delimiter it will be used as one and I will let the 
            // validate_resource function determine the validity of the resource 
            // provided.
            // Note that I am not doing a tmp_cursor--.  I want to include
            // the '=' in the resource name so that this code will fail like
            // it should.
            break;
        }
        
        tmp_cursor++;
    }
    
    char* p_resource = (char*)malloc(RESOURCE_MAX+1);
    int length = substr(code, start, tmp_cursor, p_resource, RESOURCE_MAX);
    
    if (length == -1) {
        print_error(code, tmp_cursor, RESOURCE_MAX_MSG, DEFAULT_EXIT);
    }
    
    *cursor = tmp_cursor + 2; // Move cursor to resource type
    
    return p_resource;
}

char* get_resource_type(char code[], int* cursor) {
    int start = *cursor;
    int tmp_cursor = *cursor;
    int ok = 0;

    // Find the delimiter
    while (code[tmp_cursor] != '\0') {
        switch (code[tmp_cursor]) {
            case '>' :
            case '<' :
            case '!' :
            case '=' :
                ok = 1;
                tmp_cursor--;
                break;
            case '\'' :
            case '"' :
                // They did not specify an operator...
                ok = 1;
                tmp_cursor--;
                break;
        }
        
        if (ok == 1) {
            break;
        }
        
        tmp_cursor++;
    }
    
    char* p_resource_type = (char*)malloc(RESOURCE_TYPE_MAX+1);
    
    int length = substr(code, start, tmp_cursor, p_resource_type, RESOURCE_TYPE_MAX);
    
    if (length == -1) {
        print_error(code, tmp_cursor, RESOURCE_TYPE_MAX_MSG, DEFAULT_EXIT);
    }
    
    *cursor = tmp_cursor + 1; // Move cursor to operator 
    
    return p_resource_type;
}

char* get_operator(char code[], int* cursor) {
    int start = *cursor;
    int tmp_cursor = *cursor;
    
    // Find the delimiter
    switch (code[tmp_cursor]) {
        case '>' :
        case '<' :
        case '!' :
            // Delimiter found.
            if (code[tmp_cursor+1] == '=') {
                // Operator is either a '>=', '<=' or '!='.  Either way, we 
                // need to move by one more to fetch the '='.
                tmp_cursor++;
            }
            break;
        case '=' :
            // Delimiter found.  It is just a '=' so no need
            // to move cursor since we are already here.
            break;
        
    }
    
    char* p_operator = (char*)malloc(OPERATOR_MAX+1);
    
    int length = substr(code, start, tmp_cursor, p_operator, OPERATOR_MAX);
    
    if (length == -1) {
        print_error(code, tmp_cursor, OPERATOR_MAX_MSG, EXIT_INVALID_SYNTAX);
    }
    
    *cursor = tmp_cursor + 1; // Move cursor to filter
    
    return p_operator;
}  

char* get_filter(char code[], int* cursor, char* type) {
    int start = *cursor;
    int tmp_cursor = *cursor;
    int inside_string = 0;
    char quote_type; // remember starting quote
    
    if (code[tmp_cursor] == '(') {
        type = "List";
    } else {
        type = "Scalar";
    }
    
    while (code[tmp_cursor] != '\0') {
        if ((code[tmp_cursor] == '&' || code[tmp_cursor] == '|') && inside_string == 0) {
            tmp_cursor--;
            break;
        }
        
        if (code[tmp_cursor] == '\'' || code[tmp_cursor] == '"') {
           if (inside_string == 1 && quote_type == code[tmp_cursor]) {
               // We found the ending quote so we are no longer in a string
               inside_string = 0;
           } else {
               // Found a starting quote so we are now in a string
               inside_string = 1;
               quote_type = code[tmp_cursor];
           }
       }
       
       tmp_cursor++;
    }
    
    char* p_filter = (char*)malloc(FILTER_MAX+1);
    
    int length = substr(code, start, tmp_cursor, p_filter, FILTER_MAX);
    
    if (length == -1) {
        print_error(code, tmp_cursor, FILTER_MAX_MSG, EXIT_INVALID_SYNTAX);
    }
    
    *cursor = tmp_cursor;
    
    return p_filter;
}

char* get_conjunctive(char code[], int* cursor) {    
    char* p_conjunctive = (char*)malloc(OPERATOR_MAX+1);
    
    // Length should always be 1 since we can have either an '&' or an '|'
    int length = substr(code, *cursor, *cursor, p_conjunctive, OPERATOR_MAX);
    
    if (length == -1) {
        print_error(code, *cursor, OPERATOR_MAX_MSG, EXIT_INVALID_SYNTAX);
    }
    
    *cursor += length;
    
    return p_conjunctive;
}

static int is_end(char code[], int* cursor) {
    if (code[*cursor+1] != '\0') {
        // We are not at the end of the code so we have more to process.
        // Next stop: get_resource()
        (*cursor)++;
        return 1;
    }
    
    // No more code to process since we reached a '\0'
    return 0;
}

static void validate_resource(char code[], int cursor, char value[], int length, const struct Statement* st) {
    int i = 0;
    int period = 0;
    char msg[100];
    int start = cursor-length;
    int error = 0;
    
    for (; i<length; i++) {
        switch (value[i]) {
            case '=' :
            case '>' :
            case '<' :
            case '!' :
                // I'm assuming that they are specifying an operator and not 
                // using one of these characters in their Resource name.
                error = 1;
                break;
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
                // They cannot use any of these characters in their Resources 
                // name.
                error = 2;
                break;
            case '.' :
                period++;
                break;
        }
    }
    
    int x = strlen(value);
    
    if (error == 1) {
        sprintf(msg, "Syntax error.  No type");
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    } else if (error == 2) {
        sprintf(msg, "Syntax error.  Invalid character '%c'", value[i]);
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    } else if (code[cursor] == '\0') {
        sprintf(msg, "Syntax error.  Unexpected END OF LINE");
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    } else if (x == 0) {
        sprintf(msg, "Syntax error.  No Resource");
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    } else if (period > 1) {
        sprintf(msg, "Syntax error.  Unexpected '.'");
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    } else if (period == 0) {
        sprintf(msg, "Syntax error.  Expecting '.'");
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    }
}

void validate_resource_type(char code[], int cursor, char value[], int length, const struct Statement* st) {
    int ok = 0;
    char msg[100];
    
    if (strcmp(value, "String") == 0) {
        ok = 1;
    } else if (strcmp(value, "Int") == 0) {
        ok = 1;
    } else if (strcmp(value, "Double") == 0) {
        ok = 1;
    } else if (strcmp(value, "@") == 0) {
        ok = 1;
    }
    
    if (ok == 0) {
        if (strlen(value) >= 50) {
            sprintf(msg, "Syntax error.  Invalid type");
        } else {
            sprintf(msg, "Syntax error.  Invalid type '%s'", value);
        }
        
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    }
    
    if (code[cursor] == '\0') {
        sprintf(msg, "Syntax error.  Unexpected END OF LINE");
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    }    
}

void validate_operator(char code[], int cursor, char value[], int length, const struct Statement* st) {
    int ok = 0;
    char msg[100];
    
    if (strcmp(st->type, "@") == 0) {
        if (strcmp(value, "=") != 0) {
            sprintf(msg, "Syntax error.  Expecting '='");
            print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
        }
    }
    
    if (strcmp(value, "=") == 0) {
        // I'm making assumptions here.  I'm assuming that if they specify
        // '==', '=>' or '=<', they really mean...
        
        if (code[cursor+1] == '=') {
            sprintf(msg, "Syntax error.  Use '=' instead of '=='");
            print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
        }
        
        if (code[cursor+1] == '>') {
            sprintf(msg, "Syntax error.  Use '>=' instead of '=>'");
            print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
        }
        
        if (code[cursor+1] == '<') {
            sprintf(msg, "Syntax error.  Use '<=' instead of '=<'");
            print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
        }
        
        ok = 1;
    } 
    
    if (strcmp(value, "!=") == 0) {
        ok = 1;
    } else if (strcmp(value, ">") == 0) {
        ok = 1;
    } else if (strcmp(value, "<") == 0) {
        ok = 1;
    } else if (strcmp(value, ">=") == 0) {
        ok = 1;
    } else if (strcmp(value, "<=") == 0) {
        ok = 1;
    }
    
    if (ok == 0) {
        if (strlen(value) >= 50) {
            sprintf(msg, "Syntax error.  Not a valid operator");
        } else {
            sprintf(msg, "Syntax error.  Not a valid operator: '%s'", value);
        }
        
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    }
    
    if (code[cursor] == '\0') {
        sprintf(msg, "Syntax error.  Unexpected END OF LINE");
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    }
}

void validate_filter(char code[], int cursor, char* type, char value[], int length, const struct Statement* st) {
    // Since we can have filters of different types, we need to figure out what 
    // type of filter this is; String, Double, Int, List
    
    if (strcmp(type, "@") == 0) {
        validate_var(code, cursor, value, length, st);
    } else if (value[0] == '(') {
        validate_list(code, cursor, type, value, length, st);
    } else if (strcmp(type, "String") == 0) {
        validate_string(code, cursor, value, length, st);
    } else if (strcmp(type, "Int") == 0) {
        validate_int(code, cursor, value, length, st);
    } else if (strcmp(type, "Double") == 0) {
        validate_double(code, cursor, value, length, st);
    }
}

void validate_conjunctive(char code[], int cursor, char value[], int length, const struct Statement* st) {
    char msg[100];
    
    if (value[0] != '&' && value[0] != '|') {
        sprintf(msg, "Syntax error.  Expecting '&' or '|' but got '%c' instead", value[0]);
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    }
}

void validate_var(char code[], int cursor, char value[], int length, const struct Statement* st) {
    int i = 0;
    int period = 0;
    char msg[100];
    int error = 0;
    int pos = 0;
    
    for (; i<length; i++) {
        switch (value[i]) {
            case '=' :
            case '>' :
            case '<' :
            case '!' :
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
                // They cannot use any of these characters in their Resource 
                // name.
                error = 1;
                break;
            case '.' :
                pos = i;
                period++;
                break;
        }
    }
    
    if (error == 1) {
        sprintf(msg, "Syntax error.  Invalid character '%c' for Var", value[i]);
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    } else if (period > 1) {
        sprintf(msg, "Syntax error.  Too many periods");
        print_error(code, cursor-length, msg, EXIT_INVALID_SYNTAX);
    } else if (period == 0) {
        sprintf(msg, "Syntax error.  Expecting '.'");
        print_error(code, cursor, msg, EXIT_INVALID_SYNTAX);
    }
}

void validate_string(char code[], int cursor, char value[], int length, const struct Statement* st) {
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
}

void validate_list(char code[], int cursor, char* type, char value[], int length, const struct Statement* st) {
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
}

void validate_list_string(char code[], int cursor, char value[], int length, const struct Statement* st) {
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
}

void validate_list_int(char code[], int cursor, char value[], int length, const struct Statement* st) {
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
}

void validate_list_double(char code[], int cursor, char value[], int length, const struct Statement* st) {
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
}

void validate_int(char code[], int cursor, char value[], int length, const struct Statement* st) {
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
}

void validate_double(char code[], int cursor, char value[], int length, const struct Statement* st) {
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
}

void print_error(char code[], int cursor, char msg[], int exit_code) {
    int i = 0;
    
    // We only want to show the section of code that failed
    // (50 characters or less)
    int start;
    
    if (cursor-49 <= 0) {
        start = 0;
    } else {
        start = cursor - 49;
    }
    
    char* short_code = (char*)malloc(50);
    int length = substr(code, start, start+49, short_code, 50);
    
    // Error message
    printf("ERROR: %s at character %i\n%s\n", msg, cursor+1, short_code);
    
    // Point out where error occured
    for (; i<cursor-start; i++)
        putc('-', stdout);
        
    printf("^\n");
    
    exit(exit_code);
}

void exception(char msg[], char func[], int exit_code) {
    printf("Exception: %s @ Function: %s\n", msg, func);
    exit(exit_code); 
}

int substr(char* str, int start, int end, char* substr, int max) {
    int i = 0;
    char buffer[100];
    
    for (; i<=end-start; i++) {
        if (i >= max)
            return -1;
        
        if (str[start+i] == '\0')
            break;
        
        buffer[i] = str[start+i];
    }
    
    buffer[i] = '\0';
    
    strcpy(substr, buffer);
    
    return i;
}

int clean(char value[], int length, char* str) {
    int i = 0;
    int x = 0;
    char tmp[RESOURCE_MAX+1];
    
    for (; i<length; i++) {
        if (value[i] == '\n')
            continue; // Remove line breaks, even if in a string
            
        tmp[x++] = value[i];
    }
    
    
    tmp[x] = '\0';
    
    strncpy(str, tmp, RESOURCE_MAX);
    
    return x;
}

int get_statement(char code[], int* cursor, struct Statement* sts[], int* sts_index) {
    struct Statement* pst = (struct Statement*)malloc(sizeof(struct Statement));
    char* type = (char*)malloc(sizeof(char[6]));
    get_resource(code, cursor);
    get_resource_type(code, cursor);
    get_operator(code, cursor);
    get_filter(code, cursor, type);
    
    int end = is_end(code, cursor);
    
    if (end != 0)
        get_conjunctive(code, cursor);
    
    sts[(*sts_index)++] = pst;
    
    return end;
}

int parse(char code[], int* cursor, struct Statement* sts[], int* sts_index) {
    int length;
    int new_length;
    
    length = strlen(code);    
    new_length = clean(code, length, code);
    
    while (get_statement(code, cursor, sts, sts_index) != 0);
}

void urldecode(char encoded[], char* decoded) {
    int i = 0;
    int x = 0;
    
    while (encoded[i] != '\0') {
        // RFC1738: Space characters are replaced by '+'.
        if (encoded[i] == '+') {
            decoded[x] = ' ';
            i++;
        } else if (encoded[i] == '%') {
            // RFC1738: Non-alphanumeric characters are replaced by `%HH', a 
            // percent sign and two hexadecimal digits representing the ASCII 
            // code of the character.
            char hex[2] = {};
            hex[0] = encoded[i+1]; // The first hexadecimal digit
            hex[1] = encoded[i+2]; // The second hexadecimal digit
            
            char c = hex_to_dec(hex, 2);
            
            decoded[x] = c;
            
            // Move 3 spaces since we just used %HH for our character.
            i += 3;
        } else {
            decoded[x] = encoded[i];
            i++;
        }
        
        x++;
    }
}

int hex_to_dec(char hex[], int length) {
    int i = 0;
    int dec;
    int sum = 0;
    
    while (i <= length-1) {
        // Move from right to left
        switch (hex[length-1-i]) {
            case '0' :
                dec = 0;
                break;
            case '1' :
                dec = 1;
                break;
            case '2' :
                dec = 2;
                break;
            case '3' :
                dec = 3;
                break;
            case '4' :
                dec = 4;
                break;
            case '5' :
                dec = 5;
                break;
            case '6' :
                dec = 6;
                break;
            case '7' :
                dec = 7;
                break;
            case '8' :
                dec = 8;
                break;
            case '9' :
                dec = 9;
                break;
            case 'A' :
                dec = 10;
                break;
            case 'B' :
                dec = 11;
                break;
            case 'C' :
                dec = 12;
                break;
            case 'D' :
                dec = 13;
                break;
            case 'E' :
                dec = 14;
                break;
            case 'F' :
                dec = 15;
                break;
        }
        
        sum += (dec * (pow(16, i)));
        
        i++;
    }
    
    return sum;
}
