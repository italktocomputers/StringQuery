/*

StringQuery v 0.1
 
A little query language that can be embedded in a URL.
 
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
 
Valid statement syntax: 
    Entity : Type Operator Filter
  
Entity specifies the resource to apply the filter against.
  
Type specifies the data type of the resource.  The following data types are 
supported:
    String
    Int
    Double
     
The following operators are supported:
    =
    !=
    >
    <
    >=
    <=
    ?
     
Filters can be any of the supported types or a collection of types known as 
a List.  Example comparing Entity against a single value:
 
    FirstName:String='Andrew'
     
You can concatenate multiple statements by using one of the following logical
operators:
    &
    |     
     
If I want to search for multiple names:
    FirstName:String='Andrew'|FirstName:String='Doug'
 
Which can be shortened by using a List:
    FirstName:String=('Andrew','Doug')
        
Each item in a List is separated by a ','.
 
Valid List syntax:
     (value1[,value2,...])
     

Parser Options:

-f <file>        Parse code from <file>.   
-v               Version number
-c <code>        Parse code            

To do / issues:

- Error pointer not always pointing to correct location.
- Trailing white spaces after a string throws a syntax error.
      
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATEMENTS_ARRAY_LENGTH 100
#define CODE_BUFFER_LENGTH 1000

#define MSG_NO_CODE_TO_PROCESS "No code to process"
#define EXIT_NO_CODE_TO_PROCESS 1

#define MSG_TOO_MUCH_CODE_TO_PROCESS "Too much code to process"
#define EXIT_TOO_MUCH_CODE_TO_PROCESS 2

#define MSG_UNRECOGNIZED_TOKEN "Unrecognized token"
#define EXIT_UNRECOGNIZED_TOKEN 3

#define MSG_INVALID_CHARACTER "Invalid Character"
#define EXIT_INVALID_CHARACTER 4

#define MSG_INVALID_SYNTAX "Syntax Error"
#define EXIT_INVALID_SYNTAX 5

#define MSG_RESERVED_KEYWORD "Reserved keyword"
#define EXIT_RESERVED_KEYWORD 6

#define TOKEN_ENTITY "TOKEN_ENTITY"
#define TOKEN_ENTITY_TYPE "TOKEN_ENTITY_TYPE"
#define TOKEN_OPERATOR "TOKEN_OPERATOR"
#define TOKEN_FILTER "TOKEN_FILTER"
#define TOKEN_ENTITY_OR_END "TOKEN_ENTITY_OR_END"

int parse();
int get_statement();
int is_end();
void toJSON();

void print_error(char[], int);
void exception(char[], char[], int);

char* get_entity();
char* get_entity_type();
char* get_operator();
char* get_filter(char*);
char* get_logic_op();

void validate_entity(char[], int);
void validate_entity_type(char[], int);
void validate_operator(char[], int);
void validate_filter(char*, char[], int);
void validate_logic_op(char[], int);

void validate_string(char[], int);
void validate_int(char[], int);
void validate_double(char[], int);
void validate_list(char*, char[], int);
void validate_list_string(char[], int);
void validate_list_double(char[], int);
void validate_list_int(char[], int);

int statement_index = 0;
int cursor = 0;
char code[CODE_BUFFER_LENGTH] = {};
char errMsg[100];

typedef struct Statement {
    char* entity;
    char* type;
    char* operator;
    char* filter;
    char* logic;
} Statement;

Statement* statements[STATEMENTS_ARRAY_LENGTH];

char* get_entity() {
    int start = cursor;

    while (code[cursor] != '\0') {
        if (code[cursor] == ':') {
            cursor--;
            break;
        }
        
        cursor++;
    }
    
    char entity[100] = {};
    char* p_entity = (char*)malloc(100);
    strcpy(p_entity, entity);
    
    int length = substr(code, start, cursor, &p_entity);
    
    validate_entity(p_entity, length);
    
    cursor+=2; // Increment by two because we also need to pass by the ':'
    
    return p_entity;
}

char* get_entity_type() {
    int start = cursor;

    while (code[cursor] != '\0') {
        if (code[cursor] == '?') {
            cursor--;
            break;
        }
        
        if (code[cursor] == '>') {
            cursor--;
            break;
        }
        
        if (code[cursor] == '<') {
            cursor--;
            break;
        }
        
        if (code[cursor] == '!') {
            cursor--;
            break;
        }
        
        if (code[cursor] == '=') {
            cursor--;
            break;
        }
        
        cursor++;
    }
    
    char entity_type[10] = {};
    char* p_entity_type = (char*)malloc(10);
    strcpy(p_entity_type, entity_type);
    
    int length = substr(code, start, cursor, &p_entity_type);
    
    validate_entity_type(p_entity_type, length);
    
    cursor++;
    
    return p_entity_type;
}

char* get_operator() {
    int start = cursor;
    
    while (code[cursor] != '\0') {
        if (code[cursor] == '?')
            break;
        
        if (code[cursor] == '>') {
            if (code[cursor+1] == '=') {
                cursor++;
            } 
            
            break;
        }
        
        if (code[cursor] == '<') {
            if (code[cursor+1] == '=') {
                cursor++;
            }
            
            break;
        }
        
        if (code[cursor] == '!') {
            if (code[cursor+1] == '=') {
                cursor++;
            }
            
            break;
        }
        
        if (code[cursor] == '=')
            break;
        
        cursor++;
    }
    
    char operator[3] = {};
    char* p_operator = (char*)malloc(3);
    strcpy(p_operator, operator);
    
    int length = substr(code, start, cursor, &p_operator);
    
    validate_operator(p_operator, length);
    
    cursor++;
    
    return p_operator;
}  

char* get_filter(char* type) {
    int start = cursor;
    int inside_string = 0;
    char quote_type; // remember starting quote
    
    while (code[cursor] != '\0') {
        if ((code[cursor] == '&' || code[cursor] == '|') && inside_string == 0) {
            cursor--;
            break;
        }
        
        if (code[cursor] == '\'' || code[cursor] == '"') {
           if (inside_string == 1 && quote_type == code[cursor]) {
               // We found the ending quote so we are no longer in a string
               inside_string = 0;
           } else {
               // Found a starting quote so we are now in a string
               inside_string = 1;
               quote_type = code[cursor];
           }
       }
       
       cursor++;
    }
    
    char filter[100] = {};
    char* p_filter = (char*)malloc(100);
    strcpy(p_filter, filter);
    
    int length = substr(code, start, cursor, &p_filter);
    
    validate_filter(type, p_filter, length);
    
    cursor++;
    
    return p_filter;
}

char* get_logic_op() {    
    char logic_op[2] = {};
    char* p_logic_op = (char*)malloc(2);
    strcpy(p_logic_op, logic_op);
    
    int length = substr(code, cursor-1, cursor-1, &p_logic_op);
    
    validate_logic_op(p_logic_op, length);
    
    return p_logic_op;
}

int is_end() {
    if (code[cursor+1] != '\0') {
        cursor++;
        return 1;
    }
    
    return 0;
}

void validate_entity(char value[], int length) {
    int i = 0;
    char msg[100];
    
    for (; i<length; i++) {
        switch (value[i]) {
            case '=' :
            case '>' :
            case '<' :
            case '!' :
            case '?' :
                cursor = cursor - (length) + i;
                sprintf(msg, "You forgot to specify a type");
                print_error(msg, EXIT_INVALID_CHARACTER);
        }
        
        switch (value[i]) {
            case '!' :
            case '@' :
            case '#' :
            case '$' :
            case '%' :
            case '^' :
            case '&' :
            case '*' :
            case '(' :
            case ')' :
            case '+' :
            case '-' :
            case '=' :
            case '[' :
            case ']' :
            case '{' :
            case '}' :
            case ';' :
            case '\'' :
            case ':' :
            case '"' :
            case ',' :
            case '.' :
            case '<' :
            case '>' :
            case '/' :
            case '?' :
            case '\\' :
            case '|' :
            case '~' :
            case '`' :
                sprintf(msg, "Invalid character '%c' in Entity '%s'", value[i], value);
                print_error(msg, EXIT_INVALID_CHARACTER);
        }
    }
    
    int x = strlen(value);
    
    if (x == 0) {
        sprintf(msg, "Entity must be at least one character long");
        print_error(msg, EXIT_INVALID_CHARACTER);
    }
    
    int reserved_error = 0;
    
    if (strcmp(value, "String") == 0)
        reserved_error = 1;
        
    if (strcmp(value, "Int") == 0)
        reserved_error = 1;
    
    if (strcmp(value, "Double") == 0)
        reserved_error = 1;
        
    if (reserved_error == 1) {
        sprintf(msg, MSG_RESERVED_KEYWORD);
        print_error(msg, EXIT_RESERVED_KEYWORD);
    }
}

void validate_entity_type(char value[], int length) {
    int ok = 0;
    char msg[100];
    
    if (strcmp(value, "String") == 0)
        ok = 1;
    
    if (strcmp(value, "Int") == 0)
        ok = 1;
    
    if (strcmp(value, "Double") == 0)
        ok = 1;
    
    if (ok == 0) {
        sprintf(msg, "Invalid type '%s'", value);
        print_error(msg, EXIT_INVALID_CHARACTER);
    }
}

void validate_operator(char value[], int length) {
    int ok = 0;
    char msg[100];
    
    if (strcmp(value, "=") == 0) {
        ok = 1;
        
        // We are making an assumptions here 
        if (code[cursor+1] == '=') {
            sprintf(msg, "Use '=' instead of '=='");
            print_error(msg, EXIT_INVALID_CHARACTER);
        }
        
        if (code[cursor+1] == '>') {
            sprintf(msg, "Use '>=' instead of '=>'");
            print_error(msg, EXIT_INVALID_CHARACTER);
        }
        
        if (code[cursor+1] == '<') {
            sprintf(msg, "Use '<=' instead of '=<'");
            print_error(msg, EXIT_INVALID_CHARACTER);
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
        sprintf(msg, "Not a valid operator: '%s'", value);
        print_error(msg, EXIT_INVALID_CHARACTER);
    }
}

void validate_filter(char* type, char value[], int length) {
    char msg[100];

    // Since we can have filters of different types, we need to figure out what 
    // type of filter this is; String, Double, Int, List
    
    if (value[0] == '(') {
        validate_list(type, value, length);
    } else {
        if (strcmp(type, "String") == 0) {
            validate_string(value, length);
        } else if (strcmp(type, "Int") == 0) {
            validate_int(value, length);
        } else if (strcmp(type, "Double") == 0) {
            validate_double(value, length);
        } else {
            sprintf(msg, "Unknown type: %s\n", type);
            exception(msg, "validate_filter", EXIT_INVALID_SYNTAX);
        }
    }
}

void validate_logic_op(char value[], int length) {
    char msg[100];
    
    if (value[0] != '&' && value[0] != '|') {
        cursor--;
        sprintf(msg, "Expecting '&' or '|' but got '%c' instead", value[0]);
        print_error(msg, EXIT_INVALID_CHARACTER);
    }
}

void validate_string(char value[], int length) {
    int i = 0;
    char msg[100];
    
    if (value[0] != '\'' && value[0] != '"') {
        cursor -= length;
        sprintf(msg, "String must start with a ''' or a '\"' but got '%c' instead", value[0]);
        print_error(msg, EXIT_INVALID_CHARACTER);
    }
    
    if (value[0] != value[length-1]) {
        sprintf(msg, "String must have matching ending quote but got '%c' instead", value[length]);
        print_error(msg, EXIT_INVALID_CHARACTER);
    } else {
        if (value[length-2] == '\\') {
            // Ending quote was escaped
            cursor -= length;
            sprintf(msg, "String was not closed", value[length-1]);
            print_error(msg, EXIT_INVALID_CHARACTER);
        }
    }
    
    // We need to make sure they are not using their starting/ending quote
    // within their string without escaping it.
    // i=1 to skip start quote
    for (i=1; i<length-1; i++) {
        if (value[0] == value[i]) {
            if (value[i-1] != '\\') {
                sprintf(msg, "Quote must be escaped within string", value[length]);
                print_error(msg, EXIT_INVALID_CHARACTER);
            }
        }
    }
}

void validate_list(char* type, char value[], int length) {
    int i = 0;
    int ok = 0;
    char msg[100];
    int x = 1; // Start at one to skip '('
    char buffer[100];
    
    if (value[0] != '(') {
        cursor-= length;
        sprintf(msg, "Syntax error.  List must start with a '('.  Got '%c' instead", value[0]);
        print_error(msg, EXIT_INVALID_CHARACTER);
    }
    
    if (value[length-1] != ')') {
        sprintf(msg, "Syntax error.  List must end with a ')'.  Got '%c' instead", value[length-1]);
        print_error(msg, EXIT_INVALID_CHARACTER);
    }
    
    if (strcmp(type, "Int") == 0) {
        validate_list_int(value, length);
    } else if(strcmp(type, "Double") == 0) {
        validate_list_double(value, length);
    } else if(strcmp(type, "String") == 0) {
        validate_list_string(value, length);
    } else {
        exception("Unknown list type: %s", type, EXIT_INVALID_SYNTAX);
    }
}

void validate_list_string(char value[], int length) {
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
        validate_string(token, l);
        token = strtok (NULL, ",");
    }
}


void validate_list_int(char value[], int length) {
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
        validate_int(token, l);
        token = strtok (NULL, ",");
    }
}

void validate_list_double(char value[], int length) {
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
        validate_double(token, l);
        token = strtok (NULL, ",");
    }
}

void validate_int(char value[], int length) {
    char msg[100];
    int i = 0;
    
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
                if (i == 0)
                    break;
            default:
                sprintf(msg, "Not a integer: '%s'", value);
                print_error(msg, EXIT_INVALID_CHARACTER);
        }
    }
}


void validate_double(char value[], int length) {
    char msg[100];
    int i = 0;
    int dec = 0;
    
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
                break;
            case '-' :
                if (i == 0)
                    break;
            default:
                sprintf(msg, "Not a decimal: '%s'", value);
                print_error(msg, EXIT_INVALID_CHARACTER);
        }
    }
    
    if (dec > 1) {
        sprintf(msg, "Too many decimals '%s'", value);
        print_error(msg, EXIT_INVALID_CHARACTER);
    }
}

void print_error(char msg[], int exit_code) {
    int i = 0;
    
    // Error message
    printf("ERROR: %s at character %i\n", msg, cursor+1);
    
    // Code sent to this parser
    printf("%s\n", code);
    
    // Point out where error occured
    for (; i<cursor; i++)
        putc('-', stdout);
    
    putc('^', stdout);
    putc('\n', stdout);
    
    exit(exit_code);
}

void exception(char msg[], char func[], int exit_code) {
    printf("Exception: %s @ Function: %s\n", msg, func);
    exit(exit_code); 
}

int str_length(char str[]) {
    int i = 0;
    
    while (code[i] != '\0')
        i++;
    
    return i;
}

int substr(char* str, int start, int end, char** substr) {
    int i = 0;
    char buffer[100];
    
    for (; i<=end-start; i++) {
        if (str[start+i] == '\0')
            break;    
        
        buffer[i] = str[start+i];
    }
    
    buffer[i] = '\0';
    
    strcpy(*substr, buffer);
    
    return i;
}

void toJSON() {
    int i = 0;
    
    printf("[\n");
    
    for (; i<statement_index; i++) {
        printf("  {\n");
        printf("    'Entity': '%s',\n", statements[i]->entity);
        printf("    'Type': '%s',\n", statements[i]->type);
        printf("    'Operator': '%s',\n", statements[i]->operator);
        
        if (statements[i]->filter[0] == '\'' || statements[i]->filter[0] == '"') {
            printf("    'Filter': %s,\n", statements[i]->filter);
        } else {
            printf("    'Filter': '%s',\n", statements[i]->filter);
        }
        
        printf("    'Concat': '%s'\n", statements[i]->logic);
        printf("  }");
        
         if (i < statement_index-1)
           printf(",");
           
        printf("\n");
    }
    
    printf("];\n");
} 

int get_statement() {
    char* p_entity = get_entity();
    char* p_type = get_entity_type();
    char* p_operator = get_operator();
    char* p_filter = get_filter(p_type);
    char* p_logic_op;
    
    int end = is_end();
    
    if (end != 0)
        p_logic_op = get_logic_op();
    
    Statement* pst = (Statement*)malloc(sizeof(Statement));
    
    pst->entity = p_entity;
    pst->type = p_type;
    pst->operator = p_operator;
    pst->filter = p_filter;
    pst->logic = p_logic_op;
    
    statements[statement_index++] = pst;
    
    return end;
}

int parse() {
    while (get_statement() != 0);
}

int main(int argc, const char* argv[]) {
    int i = 0;
    
    printf ("StringQuery 0.1.\n");
    
    for (i=1; i<argc; i++) { 
        if (strcmp(argv[i], "-c") == 0) {
            if (argc <= i+1) {
                exception(MSG_NO_CODE_TO_PROCESS, "main", EXIT_NO_CODE_TO_PROCESS);
            }
            
            int x = strlen(argv[i+1]);
            
            if (x >= CODE_BUFFER_LENGTH) {
                exception(MSG_TOO_MUCH_CODE_TO_PROCESS, "main", EXIT_TOO_MUCH_CODE_TO_PROCESS);
            }
            
            strcpy(code, argv[i+1]);
            parse();
        } else if (strcmp(argv[i], "-f") == 0) {
            if (argc <= i+1) {
                exception(MSG_NO_CODE_TO_PROCESS, "main", EXIT_NO_CODE_TO_PROCESS);
            } else {
                FILE *fp;
                char c;
                int x = 0;
                
                fp = fopen(argv[i+1], "r");
                
                while ((c = fgetc(fp)) != EOF) {
                    if (x >= CODE_BUFFER_LENGTH) {
                        exception(MSG_TOO_MUCH_CODE_TO_PROCESS, "main", EXIT_TOO_MUCH_CODE_TO_PROCESS);
                    }
                     
                    code[x++] = c;
                }
                
                parse();
            }
        }
    }
    
    toJSON();
    
    return 0;
}
