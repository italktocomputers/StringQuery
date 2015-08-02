#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "translators/JSON.c"
#include "translators/SQL.c"

#define STATEMENTS_ARRAY_LENGTH 1000
#define CODE_BUFFER_LENGTH 50000

#define MSG_NO_CODE_TO_PROCESS "No code to process"
#define EXIT_NO_CODE_TO_PROCESS 2

#define MSG_TOO_MUCH_CODE_TO_PROCESS "Too much code to process"
#define EXIT_TOO_MUCH_CODE_TO_PROCESS 3

#define MSG_UNRECOGNIZED_TOKEN "Unrecognized token"
#define EXIT_UNRECOGNIZED_TOKEN 4

#define MSG_INVALID_CHARACTER "Invalid Character"
#define EXIT_INVALID_CHARACTER 5

#define MSG_INVALID_SYNTAX "Syntax Error"
#define EXIT_INVALID_SYNTAX 6

#define MSG_RESERVED_KEYWORD "Reserved keyword"
#define EXIT_RESERVED_KEYWORD 7

#define MSG_NO_EXPORT_TYPE "You have not specified a export type"
#define MSG_NO_EXPORT_SUPPORT "Supported export formats: JSON, SQL"
#define EXIT_NO_EXPORT_TYPE 8

#define MSG_NO_CODE_SWITCH "Must specify --code or --file"
#define EXIT_NO_CODE_SWITCH 9

#define TOKEN_ENTITY "TOKEN_ENTITY"
#define TOKEN_ENTITY_TYPE "TOKEN_ENTITY_TYPE"
#define TOKEN_OPERATOR "TOKEN_OPERATOR"
#define TOKEN_FILTER "TOKEN_FILTER"
#define TOKEN_ENTITY_OR_END "TOKEN_ENTITY_OR_END"

#define EXPORT_LENGTH 10

static int parse(char[], int*, struct Statement*[], int*);
static int get_statement(char[], int*, struct Statement*[], int*);
static int is_end(char[], int*);
static int substr(char*, int, int, char*, int);
static int remove_newline(char[], int, char*);

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
    int l = 0;
    int x = *cursor;
    
    while (code[x] != '\0') {
        if (code[x] == ':') {
            l--;
            break;
        }
        
        l++; 
        x++;
    }
    
    char* p_entity = (char*)malloc(ENTITY_MAX+1);
    int l2 = substr(code, *cursor, *cursor+l, p_entity, ENTITY_MAX);
    
    if (l2 == -1) {
        print_error(code, *cursor, ENTITY_MAX_MSG, EXIT_INVALID_CHARACTER);
    }
    
    l2 = remove_newline(p_entity, l2, p_entity);
    
    validate_entity(code, *cursor, p_entity, l2);
    
    st->entity = p_entity;
    
    *cursor += l + 2; // +2 to move pass ':'
}

static void get_entity_type(char code[], int* cursor, struct Statement* st) {
    int l = 0;
    int x = *cursor;

    while (code[x] != '\0') {
        if (code[x] == '?') {
            l--;
            break;
        }
        
        if (code[x] == '>') {
            l--;
            break;
        }
        
        if (code[x] == '<') {
            l--;
            break;
        }
        
        if (code[x] == '!') {
            l--;
            break;
        }
        
        if (code[x] == '=') {
            l--;
            break;
        }
        
        l++;
        x++;
    }
    
    char* p_entity_type = (char*)malloc(ENTITY_TYPE_MAX+1);
    
    int l2 = substr(code, *cursor, *cursor+l, p_entity_type, ENTITY_TYPE_MAX);
    
    if (l2 == -1) {
        print_error(code, *cursor, ENTITY_TYPE_MAX_MSG, EXIT_INVALID_CHARACTER);
    }
    
    validate_entity_type(code, *cursor, p_entity_type, l2);
    
    st->type = p_entity_type;
    
    *cursor += l + 1;
}

static void get_operator(char code[], int* cursor, struct Statement* st) {
    int l = 0;
    int x = *cursor;
    
    while (code[x] != '\0') {
        if (code[x] == '?')
            break;
        
        if (code[x] == '>') {
            if (code[x+1] == '=') {
                l++;
            } 
            
            break;
        }
        
        if (code[x] == '<') {
            if (code[x+1] == '=') {
                l++;
            }
            
            break;
        }
        
        if (code[x] == '!') {
            if (code[x+1] == '=') {
                l++;
            }
            
            break;
        }
        
        if (code[x] == '=')
            break;
        
        l++;
        x++;
    }
    
    char* p_operator = (char*)malloc(OPERATOR_MAX+1);
    
    int l2 = substr(code, *cursor, *cursor+l, p_operator, OPERATOR_MAX);
    
    if (l2 == -1) {
        print_error(code, *cursor, OPERATOR_MAX_MSG, EXIT_INVALID_CHARACTER);
    }
    
    validate_operator(code, *cursor, p_operator, l2);
    
    st->operator = p_operator;
    
    *cursor += l + 1;
}  

static void get_filter(char code[], int* cursor, struct Statement* st, char* type) {
    int l = 0;
    int x = *cursor;
    int inside_string = 0;
    char quote_type; // remember starting quote
    
    if (code[x] == '(') {
        st->filter_type = "List";
    } else {
        st->filter_type = "Scalar";
    }
    
    while (code[x] != '\0') {
        if ((code[x] == '&' || code[x] == '|') && inside_string == 0) {
            l--;
            break;
        }
        
        if (code[x] == '\'' || code[x] == '"') {
           if (inside_string == 1 && quote_type == code[x]) {
               // We found the ending quote so we are no longer in a string
               inside_string = 0;
           } else {
               // Found a starting quote so we are now in a string
               inside_string = 1;
               quote_type = code[x];
           }
       }
       
       l++;
       x++;
    }
    
    char* p_filter = (char*)malloc(FILTER_MAX+1);
    
    int l2 = substr(code, *cursor, *cursor+l, p_filter, FILTER_MAX);
    
    if (l2 == -1) {
        print_error(code, *cursor, FILTER_MAX_MSG, EXIT_INVALID_CHARACTER);
    }
    
    validate_filter(code, *cursor, type, p_filter, l2);
    
    st->filter = p_filter;
    
    *cursor += l + 1;
}

static void get_logic_op(char code[], int* cursor, struct Statement* st) {    
    char* p_logic_op = (char*)malloc(OPERATOR_MAX+1);
    
    int l = substr(code, *cursor-1, *cursor-1, p_logic_op, OPERATOR_MAX);
    
    if (l == -1) {
        print_error(code, *cursor, OPERATOR_MAX_MSG, EXIT_INVALID_CHARACTER);
    }
    
    validate_logic_op(code, *cursor, p_logic_op, l);
    
    st->concat = p_logic_op;
    
    *cursor += l - 1;
}

static int is_end(char code[], int* cursor) {
    if (code[*cursor+1] != '\0') {
        (*cursor)++;
        return 1;
    }
    
    return 0;
}

static void validate_entity(char code[], int cursor, char value[], int length) {
    int i = 0;
    char msg[100];
    int start = cursor;
    
    for (; i<length; i++) {
        switch (value[i]) {
            case '=' :
            case '>' :
            case '<' :
            case '!' :
            case '?' :
                sprintf(msg, "You forgot to specify a type");
                print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
        }
        
        switch (value[i]) {
            case '!' :
            case '&' :
            case '*' :
            case '(' :
            case ')' :
            case '+' :
            case '-' :
            case '=' :
            case '\'' :
            case ':' :
            case '"' :
            case ',' :
            case '<' :
            case '>' :
            case '?' :
            case '\\' :
                sprintf(msg, "Invalid character '%c'", value[i]);
                print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
        }
    }
    
    int x = strlen(value);
    
    if (x == 0) {
        sprintf(msg, "Entity must be at least one character long");
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    }
}

static void validate_entity_type(char code[], int cursor, char value[], int length) {
    int ok = 0;
    char msg[100];
    
    if (strcmp(value, "String") == 0)
        ok = 1;
    
    if (strcmp(value, "Int") == 0)
        ok = 1;
    
    if (strcmp(value, "Double") == 0)
        ok = 1;
    
    if (ok == 0) {
        if (strlen(value) >= 50) {
            sprintf(msg, "Invalid type");
        } else {
            sprintf(msg, "Invalid type '%s'", value);
        }
        
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    }
}

static void validate_operator(char code[], int cursor, char value[], int length) {
    int ok = 0;
    char msg[100];
    
    if (strcmp(value, "=") == 0) {
        ok = 1;
        
        // We are making assumptions here 
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
    } 
    
    if (strcmp(value, "!=") == 0)
        ok = 1;
    
    if (strcmp(value, ">") == 0)
        ok = 1;
    
    if (strcmp(value, "<") == 0)
        ok = 1;
    
    if (strcmp(value, ">=") == 0)
        ok = 1;
    
    if (strcmp(value, "<=") == 0)
        ok = 1;
    
    if (ok == 0) {
        if (strlen(value) >= 50) {
            sprintf(msg, "Not a valid operator");
        } else {
            sprintf(msg, "Not a valid operator: '%s'", value);
        }
        
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    }
}

static void validate_filter(char code[], int cursor, char* type, char value[], int length) {
    char msg[100];

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
        } else {
            if (strlen(value) >= 50) {
                sprintf(msg, "Unknown type\n");
            } else {
                sprintf(msg, "Unknown type: %s\n", type);
            }
            
            exception(msg, "validate_filter", EXIT_INVALID_SYNTAX);
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
    
    if (value[0] != '\'' && value[0] != '"') {
        sprintf(msg, "String must start with a ''' or a '\"' but got '%c' instead", value[0]);
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    }
    
    if (value[0] != value[length-1]) {
        sprintf(msg, "String must have matching ending quote but got '%c' instead", value[length]);
        print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
    } else {
        if (value[length-2] == '\\') {
            // Ending quote was escaped
            sprintf(msg, "String was not closed", value[length-1]);
            print_error(code, cursor, msg, EXIT_INVALID_CHARACTER);
        }
    }
    
    // We need to make sure they are not using their starting/ending quote
    // within their string without escaping it.
    // i=1 to skip start quote
    for (i=1; i<length-1; i++) {
        if (value[0] == value[i]) {
            if (value[i-1] != '\\') {
                sprintf(msg, "Quote must be escaped within string", value[length]);
                print_error(code, cursor+i, msg, EXIT_INVALID_CHARACTER);
            }
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
    char msg[100];
    int i = 0;
    int error = 0;
    
    for (; i<length; i++) {
        if (error == 1)
            break;
            
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
    char msg[100];
    int i = 0;
    int dec = 0;
    int error = 0;
    
    for (i=0; i<length; i++) {
        if (error != 0)
            break;
            
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
    printf("ERROR: %s at character %i\n%s\n", msg, start+49, short_code);
    
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

static int remove_newline(char value[], int length, char* str) {
    int i = 0;
    int x = 0;
    char tmp[ENTITY_MAX];
    
    for (; i<length; i++) {
        if (value[i] == '\n')
            continue;
            
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
    while (get_statement(code, cursor, sts, sts_index) != 0);
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
    struct Statement* sts[STATEMENTS_ARRAY_LENGTH];
    
    for (i=1; i<argc; i++) { 
        if (strcmp(argv[i], "--code") == 0) {
            code_switch = 1;
            
            if (argv[i+1] == NULL) {
                printf("ERROR: %s\n", MSG_NO_CODE_TO_PROCESS);
                exit(EXIT_NO_CODE_TO_PROCESS);
            }
            
            int x = strlen(argv[i+1]);
            
            if (x >= CODE_BUFFER_LENGTH) {
                printf("ERROR: %s\n", MSG_TOO_MUCH_CODE_TO_PROCESS);
                exit(EXIT_TOO_MUCH_CODE_TO_PROCESS);
            }
            
            strncpy(code, argv[i+1], CODE_BUFFER_LENGTH);
            parse(code, &cursor, sts, &sts_index);
        } else if (strcmp(argv[i], "--file") == 0) {
            file_switch = 1;
            
            if (argv[i+1] == NULL) {
                printf("ERROR: %s\n", MSG_NO_CODE_TO_PROCESS);
                exit(EXIT_NO_CODE_TO_PROCESS);
            } else {
                FILE *fp;
                char c;
                int x = 0;
                
                fp = fopen(argv[i+1], "r");
                
                while ((c = fgetc(fp)) != EOF) {
                    if (x >= CODE_BUFFER_LENGTH) {
                        printf("ERROR: %s\n", MSG_TOO_MUCH_CODE_TO_PROCESS);
                        exit(EXIT_TOO_MUCH_CODE_TO_PROCESS);
                    }
                     
                    code[x++] = c;
                }
                
                parse(code, &cursor, sts, &sts_index);
            }
        } else if (strcmp(argv[i], "--export") == 0) {
            export_switch = 1;
            
            if (argv[i+1] == NULL) {
                printf("ERROR: %s\n", MSG_NO_EXPORT_TYPE);
                exit(EXIT_NO_EXPORT_TYPE);
            }
            
            strncpy(export_type, argv[i+1], EXPORT_LENGTH);
        }
    }
    
    if (code_switch == 0 && file_switch == 0) {
        printf("ERROR: %s\n", MSG_NO_CODE_SWITCH);
        exit(EXIT_NO_CODE_SWITCH);
    }
    
    if (strcmp(export_type, "JSON") == 0) {
        toJSON(sts, sts_index);
    } else if(strcmp(export_type, "SQL") == 0) {
        toSQL(sts, sts_index);
    } else {
        printf("ERROR: %s\n", MSG_NO_EXPORT_SUPPORT);
        exit(EXIT_NO_EXPORT_TYPE);
    }
    
    return 0;
}
