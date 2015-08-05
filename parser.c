#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include "parser.h"
#include "translators/JSON.c"
#include "translators/SQL.c"

#define STATEMENTS_ARRAY_LENGTH 1000
#define CODE_BUFFER_LENGTH 50000

#define MSG_NO_CODE_TO_PROCESS "No code to process"
#define MSG_TOO_MUCH_CODE_TO_PROCESS "Too much code to process"

#define MSG_UNRECOGNIZED_TOKEN "Unrecognized token"
#define EXIT_UNRECOGNIZED_TOKEN 2

#define MSG_INVALID_CHARACTER "Invalid Character"
#define EXIT_INVALID_CHARACTER 3

#define MSG_INVALID_SYNTAX "Syntax Error"
#define EXIT_INVALID_SYNTAX 4

#define MSG_RESERVED_KEYWORD "Reserved keyword"
#define EXIT_RESERVED_KEYWORD 5

#define MSG_NO_EXPORT_TYPE "You have not specified a export type"
#define MSG_NO_EXPORT_SUPPORT "Supported export formats: JSON, SQL"
#define MSG_NO_CODE_SWITCH "Must specify --code or --file"
#define MSG_NO_CODE_FORMAT "You have not specified a code format"

#define TOKEN_ENTITY "TOKEN_ENTITY"
#define TOKEN_ENTITY_TYPE "TOKEN_ENTITY_TYPE"
#define TOKEN_OPERATOR "TOKEN_OPERATOR"
#define TOKEN_FILTER "TOKEN_FILTER"
#define TOKEN_ENTITY_OR_END "TOKEN_ENTITY_OR_END"

#define EXPORT_ARG_LENGTH 10
#define CODE_FORMAT_ARG_LENGTH 50

static int parse(char[], int*, struct Statement*[], int*);
static int get_statement(char[], int*, struct Statement*[], int*);
static int is_end(char[], int*);
static int substr(char*, int, int, char*, int);
static int clean(char[], int, char*);

int hex_to_dec(char[], int);

static void urldecode(char[], char*);
static void print_error(char[], int, char[], int);
static void exception(char[], char[], int);

static void get_entity(char[], int*, struct Statement*);
static void get_entity_type(char[], int*, struct Statement*);
static void get_operator(char[], int*, struct Statement*);
static void get_filter(char[], int*, struct Statement*, char*);
static void get_logic_op(char[], int*, struct Statement*);

static void validate_entity(char[], int, char[], int);
static void validate_entity_type(char[], int, char[], int);
static void validate_operator(char[], int, char[], int);
static void validate_filter(char[], int, char*, char[], int);
static void validate_logic_op(char[], int, char[], int);

static void validate_string(char[], int, char[], int);
static void validate_int(char[], int, char[], int);
static void validate_double(char[], int, char[], int);
static void validate_list(char[], int, char*, char[], int);
static void validate_list_string(char[], int, char[], int);
static void validate_list_double(char[], int, char[], int);
static void validate_list_int(char[], int, char[], int);

static void get_entity(char code[], int* cursor, struct Statement* st) {
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
            // validate_entity function determine the validity of the entity 
            // provided.
            // Note that I am not doing a tmp_cursor--.  I want to include
            // the '=' in the entity name so that this code will fail like
            // it should.
            break;
        }
        
        tmp_cursor++;
    }
    
    char* p_entity = (char*)malloc(ENTITY_MAX+1);
    int length = substr(code, start, tmp_cursor, p_entity, ENTITY_MAX);
    
    if (length == -1) {
        print_error(code, tmp_cursor, ENTITY_MAX_MSG, DEFAULT_EXIT);
    }
    
    validate_entity(code, tmp_cursor, p_entity, length);
    
    st->entity = p_entity;
    
    *cursor = tmp_cursor + 2; // Move cursor to entity type
}

static void get_entity_type(char code[], int* cursor, struct Statement* st) {
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
    
    char* p_entity_type = (char*)malloc(ENTITY_TYPE_MAX+1);
    
    int length = substr(code, start, tmp_cursor, p_entity_type, ENTITY_TYPE_MAX);
    
    if (length == -1) {
        print_error(code, tmp_cursor, ENTITY_TYPE_MAX_MSG, DEFAULT_EXIT);
    }
    
    validate_entity_type(code, tmp_cursor, p_entity_type, length);
    
    st->type = p_entity_type;
    
    *cursor = tmp_cursor + 1; // Move cursor to operator 
}

static void get_operator(char code[], int* cursor, struct Statement* st) {
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
        print_error(code, tmp_cursor, OPERATOR_MAX_MSG, EXIT_INVALID_CHARACTER);
    }
    
    validate_operator(code, tmp_cursor, p_operator, length);
    
    st->operator = p_operator;
    
    *cursor = tmp_cursor + 1; // Move cursor to filter
}  

static void get_filter(char code[], int* cursor, struct Statement* st, char* type) {
    int start = *cursor;
    int tmp_cursor = *cursor;
    int inside_string = 0;
    char quote_type; // remember starting quote
    
    if (code[tmp_cursor] == '(') {
        st->filter_type = "List";
    } else {
        st->filter_type = "Scalar";
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
        print_error(code, tmp_cursor, FILTER_MAX_MSG, EXIT_INVALID_CHARACTER);
    }
    
    validate_filter(code, tmp_cursor, type, p_filter, length);
    
    st->filter = p_filter;
    
    *cursor = tmp_cursor;
}

static void get_logic_op(char code[], int* cursor, struct Statement* st) {    
    char* p_logic_op = (char*)malloc(OPERATOR_MAX+1);
    
    // Length should always be 1 since we can have either an '&' or an '|'
    int length = substr(code, *cursor, *cursor, p_logic_op, OPERATOR_MAX);
    
    if (length == -1) {
        print_error(code, *cursor, OPERATOR_MAX_MSG, EXIT_INVALID_CHARACTER);
    }
    
    validate_logic_op(code, *cursor, p_logic_op, length);
    
    st->concat = p_logic_op;
    
    *cursor += length;
}

static int is_end(char code[], int* cursor) {
    if (code[*cursor+1] != '\0') {
        // We are not at the end of the code so we have more to process.
        // Next stop: get_entity()
        (*cursor)++;
        return 1;
    }
    
    // No more code to process since we reached a '\0'
    return 0;
}

static void validate_entity(char code[], int cursor, char value[], int length) {
    int i = 0;
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
                // using one of these characters in their Entity name.
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
                // They cannot use any of these characters in their Entity 
                // name.
                error = 2;
                break;
        }
    }
    
    int x = strlen(value);
    
    if (code[cursor] == '\0') 
        error = 3;
    
    if (x == 0)
        error = 4;
    
    if (error == 1) {
        sprintf(msg, "You forgot to specify a type");
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    } else if (error == 2) {
        sprintf(msg, "Invalid character '%c' for Entity", value[i]);
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    } else if (error == 3) {
        sprintf(msg, "Unexpected END OF LINE");
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    } else if (error == 4) {
        sprintf(msg, "Entity must be at least one character long");
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    }
}

static void validate_entity_type(char code[], int cursor, char value[], int length) {
    int ok = 0;
    char msg[100];
    
    if (strcmp(value, "String") == 0) {
        ok = 1;
    } else if (strcmp(value, "Int") == 0) {
        ok = 1;
    } else if (strcmp(value, "Double") == 0) {
        ok = 1;
    }
    
    if (ok == 0) {
        if (strlen(value) >= 50) {
            sprintf(msg, "Invalid type");
        } else {
            sprintf(msg, "Invalid type '%s'", value);
        }
        
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    }
    
    if (code[cursor] == '\0') {
        sprintf(msg, "Unexpected END OF LINE");
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    }    
}

static void validate_operator(char code[], int cursor, char value[], int length) {
    int ok = 0;
    char msg[100];
    
    if (strcmp(value, "=") == 0) {
        // I'm making assumptions here.  I'm assuming that if they specify
        // '==', '=>' or '=<', they really mean...
        
        if (code[cursor+1] == '=') {
            sprintf(msg, "Use '=' instead of '=='");
            print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
        }
        
        if (code[cursor+1] == '>') {
            sprintf(msg, "Use '>=' instead of '=>'");
            print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
        }
        
        if (code[cursor+1] == '<') {
            sprintf(msg, "Use '<=' instead of '=<'");
            print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
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
            sprintf(msg, "Not a valid operator");
        } else {
            sprintf(msg, "Not a valid operator: '%s'", value);
        }
        
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    }
    
    if (code[cursor] == '\0') {
        sprintf(msg, "Unexpected END OF LINE");
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    }
}

static void validate_filter(char code[], int cursor, char* type, char value[], int length) {
    // Since we can have filters of different types, we need to figure out what 
    // type of filter this is; String, Double, Int, List
    
    if (value[0] == '(') {
        validate_list(code, cursor, type, value, length);
    } else {
        if (strcmp(type, "String") == 0) {
            validate_string(code, cursor, value, length);
        } else if (strcmp(type, "Int") == 0) {
            validate_int(code, cursor, value, length);
        } else if (strcmp(type, "Double") == 0) {
            validate_double(code, cursor, value, length);
        }
    }
}

static void validate_logic_op(char code[], int cursor, char value[], int length) {
    char msg[100];
    
    if (value[0] != '&' && value[0] != '|') {
        sprintf(msg, "Expecting '&' or '|' but got '%c' instead", value[0]);
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    }
}

static void validate_string(char code[], int cursor, char value[], int length) {
    int i = 0;
    char msg[100];
    char pos = 1;
    
    if (value[0] != '\'' && value[0] != '"') {
        sprintf(msg, "String must start with a quote but got '%c' instead", value[0]);
        print_error(code, cursor-length, msg, EXIT_INVALID_CHARACTER);
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
        sprintf(msg, "String must have matching ending quote but got '%c' instead", value[length]);
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
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
                sprintf(msg, "Syntax error", value[length]);
                print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
        }
    }
}

static void validate_list(char code[], int cursor, char* type, char value[], int length) {
    char msg[100];
    
    if (value[0] != '(') {
        sprintf(msg, "Syntax error.  List must start with a '('.  Got '%c' instead", value[0]);
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    }
    
    if (value[length-1] != ')') {
        sprintf(msg, "Syntax error.  List must end with a ')'.  Got '%c' instead", value[length-1]);
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    }
    
    if (strcmp(type, "Int") == 0) {
        validate_list_int(code, cursor, value, length);
    } else if(strcmp(type, "Double") == 0) {
        validate_list_double(code, cursor, value, length);
    } else if(strcmp(type, "String") == 0) {
        validate_list_string(code, cursor, value, length);
    } else {
        exception("Unknown list type: %s", type, EXIT_INVALID_SYNTAX);
    }
}

static void validate_list_string(char code[], int cursor, char value[], int length) {
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
        validate_string(code, cursor, token, l);
        token = strtok (NULL, ",");
    }
}


static void validate_list_int(char code[], int cursor, char value[], int length) {
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
        validate_int(code, cursor, token, l);
        token = strtok (NULL, ",");
    }
}

static void validate_list_double(char code[], int cursor, char value[], int length) {
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
        validate_double(code, cursor, token, l);
        token = strtok (NULL, ",");
    }
}

static void validate_int(char code[], int cursor, char value[], int length) {
    int i = 0;
    int error = 0;
    char msg[100];
    
    if (length == 0) {
        sprintf(msg, "Not a integer");
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
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
            sprintf(msg, "Not a integer");
        } else {
            sprintf(msg, "Not a integer: '%s'", value);
        }
        
        print_error(code, cursor+i, msg, EXIT_INVALID_CHARACTER);
    }
}


static void validate_double(char code[], int cursor, char value[], int length) {
    int i = 0;
    int dec = 0;
    int error = 0;
    char msg[100];
    
    if (length == 0) {
        sprintf(msg, "Not a double");
        print_error(code, cursor+i, msg, EXIT_INVALID_CHARACTER);
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
            sprintf(msg, "Not a decimal");
        } else {
            sprintf(msg, "Not a decimal: '%s'", value);
        }
        
        print_error(code, cursor+i, msg, EXIT_INVALID_CHARACTER);
    } else if (error == 2) {
        if (strlen(value) >= 50) {
            sprintf(msg, "Too many decimals");
        } else {
            sprintf(msg, "Too many decimals '%s'", value);
        }
        
        print_error(code, cursor+i, msg, EXIT_INVALID_CHARACTER);
    }
}

static void print_error(char code[], int cursor, char msg[], int exit_code) {
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

static void exception(char msg[], char func[], int exit_code) {
    printf("Exception: %s @ Function: %s\n", msg, func);
    exit(exit_code); 
}

static int substr(char* str, int start, int end, char* substr, int max) {
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

static int clean(char value[], int length, char* str) {
    int i = 0;
    int x = 0;
    char tmp[ENTITY_MAX];
    
    for (; i<length; i++) {
        if (value[i] == '\n')
            continue; // Remove line breaks, even if in a string
            
        tmp[x++] = value[i];
    }
    
    
    tmp[x] = '\0';
    
    strncpy(str, tmp, ENTITY_MAX);
    
    return x;
}

static int get_statement(char code[], int* cursor, struct Statement* sts[], int* sts_index) {
    struct Statement* pst = (struct Statement*)malloc(sizeof(struct Statement));
    
    get_entity(code, cursor, pst);
    get_entity_type(code, cursor, pst);
    get_operator(code, cursor, pst);
    get_filter(code, cursor, pst, pst->type);
    
    int end = is_end(code, cursor);
    
    if (end != 0)
        get_logic_op(code, cursor, pst);
    
    sts[(*sts_index)++] = pst;
    
    return end;
}

static int parse(char code[], int* cursor, struct Statement* sts[], int* sts_index) {
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

int main(int argc, const char* argv[]) {
    int i = 0;
    int sts_index = 0;
    int cursor = 0;
    char code[CODE_BUFFER_LENGTH] = {};
    char export_type[10] = "JSON";
    int code_switch = 0;
    int file_switch = 0;
    int export_switch = 0;
    int code_format_switch = 0;
    char code_format[50];
    
    struct Statement* sts[STATEMENTS_ARRAY_LENGTH];
    
    for (i=1; i<argc; i++) { 
        if (strcmp(argv[i], "--code") == 0) {
            code_switch = 1;
            
            if (argv[i+1] == NULL) {
                printf("ERROR: %s\n", MSG_NO_CODE_TO_PROCESS);
                exit(DEFAULT_EXIT);
            }
            
            int x = strlen(argv[i+1]);
            
            if (x >= CODE_BUFFER_LENGTH) {
                printf("ERROR: %s\n", MSG_TOO_MUCH_CODE_TO_PROCESS);
                exit(DEFAULT_EXIT);
            }
            
            strncpy(code, argv[i+1], CODE_BUFFER_LENGTH);
        } else if (strcmp(argv[i], "--file") == 0) {
            file_switch = 1;
            
            if (argv[i+1] == NULL) {
                printf("ERROR: %s\n", MSG_NO_CODE_TO_PROCESS);
                exit(DEFAULT_EXIT);
            } else {
                FILE *fp;
                char c;
                int x = 0;
                
                fp = fopen(argv[i+1], "r");
                
                while ((c = fgetc(fp)) != EOF) {
                    if (x >= CODE_BUFFER_LENGTH) {
                        printf("ERROR: %s\n", MSG_TOO_MUCH_CODE_TO_PROCESS);
                        exit(DEFAULT_EXIT);
                    }
                     
                    code[x++] = c;
                }
            }
        } else if (strcmp(argv[i], "--export") == 0) {
            export_switch = 1;
            
            if (argv[i+1] == NULL) {
                printf("ERROR: %s\n", MSG_NO_EXPORT_TYPE);
                exit(DEFAULT_EXIT);
            }
            
            strncpy(export_type, argv[i+1], EXPORT_ARG_LENGTH);
        } else if (strcmp(argv[i], "--code-format") == 0) {
            code_format_switch = 1;
            
            if (argv[i+1] == NULL) {
                printf("ERROR: %s\n", MSG_NO_CODE_FORMAT);
                exit(DEFAULT_EXIT);
            }
            
            strncpy(code_format, argv[i+1], CODE_FORMAT_ARG_LENGTH);
        }
    }
    
    if (code_switch == 0 && file_switch == 0) {
        printf("ERROR: %s\n", MSG_NO_CODE_SWITCH);
        exit(DEFAULT_EXIT);
    }
    
    char decoded_code[CODE_BUFFER_LENGTH] = {};
    
    if (code_format_switch == 1 && strcmp(code_format, "urlencoded") == 0) {
        urldecode(code, decoded_code); 
    } else {
        strncpy(decoded_code, code, CODE_BUFFER_LENGTH);
    }
    
    parse(decoded_code, &cursor, sts, &sts_index);
    
    if (strcmp(export_type, "JSON") == 0) {
        toJSON(sts, sts_index);
    } else if(strcmp(export_type, "SQL") == 0) {
        toSQL(sts, sts_index);
    } else {
        printf("ERROR: %s\n", MSG_NO_EXPORT_SUPPORT);
        exit(DEFAULT_EXIT);
    }
    
    return 0;
}
