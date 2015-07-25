/*
  MezeyQuery v 0.1
 
  A little query language that can be embedded in a URL.
 
  Copyright (c) 2015, Andrew Schools <andrewschools@me.com>
  Permission is hereby granted, free of charge, to any
  person obtaining a copy of this software and associated
  documentation files (the "Software"), to deal in the
  Software without restriction, including without
  limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies
  of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the
  following conditions:
  The above copyright notice and this permission notice
  shall be included in all copies or substantial portions
  of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
 
  Valid syntax: 
      Entity:Type=Filter[&Entity:Type=Filter]
  
  Entity specifies the resource to apply the filter against.
  
  Type specifies the data type of the resource.  If the Entity is
  of type String, then the filter will do a string comparison.
  
  Filter values can be any of the following data types:
     - String
     - Int
     - Double
     - Date
     -  ()   Entity must equal ALL values in list
     - !()   Entity must NOT equal any values in list
     - ^()   Entity must equal at least one value in list
 
     Strings do NOT need to be quoted, however, if they contain any special 
     characters they must be quoted or a parse error will occur.  
     
     For example, the following string does not need quotes:
     
         Dept:String=Engineering Department
 
     However, the following string DOES need quotes:
 
         Dept:String='Engineering & Mathamatics'
           
     If quotes were not used in the example above, the parser would end the 
     filter after finding '&' and consider 'Mathamatics' an Entity.
 
     Each item in a List is separated by a ','.  A list must contain at least 
     two items.  
 
  Valid List syntax:
      (value1[,value2,...])
      
  Examples:
       
      FirstName:String=^('Andrew', 'Jeff', 'Laura') 
      
      Salary:Int=(45000-65000)
      
      Birds:String=('%A'-'%C')
      
      
   
  Todo:
      - Keep track of character # for better debugging.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_BUFFER_LENGTH 100
#define CODE_BUFFER_LENGTH 1000

#define MSG_NO_CODE_TO_PROCESS "No code to process"
#define EXIT_NO_CODE_TO_PROCESS 1

#define MSG_UNRECOGNIZED_TOKEN "Unrecognized token"
#define EXIT_UNRECOGNIZED_TOKEN 2

#define MSG_INVALID_CHARACTER "Invalid Character"
#define EXIT_INVALID_CHARACTER 3

#define TOKEN_ENTITY "TOKEN_ENTITY"
#define TOKEN_ENTITY_TYPE "TOKEN_ENTITY_TYPE"
#define TOKEN_FILTER "TOKEN_FILTER"

#define TOKEN_LIST_EQUAL "TOKEN_LIST_EQUAL"
#define TOKEN_LIST_NOT_EQUAL "TOKEN_LIST_NOT_EQUAL"
#define TOKEN_LIST_OR "TOKEN_LIST_OR"
#define TOKEN_ASSIGNMENT "TOKEN_ASSIGNMENT"


#define TOKEN_STRING "TOKEN_STRING"
#define TOKEN_INT "TOKEN_INT"
#define TOKEN_DOUBLE "TOKEN_DOUBLE"

#define TOKEN_ENTITY_DELIMITER "TOKEN_ENTITY_DELIMITER"
#define TOKEN_ENTITY_TYPE_DELIMITER "TOKEN_ENTITY_TYPE_DELIMITER"
#define TOKEN_FILTER_DELIMITER "TOKEN_FILTER_DELIMITER"
#define TOKEN_FILTER_ITEM_DELIMITER "TOKEN_FILTER_ITEM_DELIMITER"
#define TOKEN_RANGE_DELIMITER "TOKEN_RANGE_DELIMITER"

#define TOKEN_ENTITY_DELIMITER_VALUE ':'
#define TOKEN_ENTITY_TYPE_DELIMITER_VALUE '='
#define TOKEN_FILTER_DELIMITER_VALUE '&'
#define TOKEN_FILTER_ITEM_DELIMITER_VALUE ','
#define TOKEN_RANGE_DELIMITER_VALUE '-'

int parse(const char*);
void get_token(char**);
void print_err(char*);
void get_delimiter(char*, char*);
void validate(char*, char[], int);
void validate_entity(char[], int);
void validate_entity_type(char[], int);
void validate_filter(char[], int);
void validate_quoted_string(char[], int);
void validate_list(char[], int);
void validate_range(char[], int);
int validate_int(char[], int);
int validate_double(char[], int);
int lookout(char[], int, char, char);

int nodeIndex = 0;

typedef struct node {
    char name[50];
    char value[100];
} node;

node* token_array[TOKEN_BUFFER_LENGTH];

void get_token(char** token) {
    if (strcmp(*token, TOKEN_ENTITY) == 0) {
        *token = TOKEN_ENTITY_TYPE;
    } else if (strcmp(*token, TOKEN_ENTITY_TYPE) == 0) {
        *token = TOKEN_FILTER;
    } else if (strcmp(*token, TOKEN_FILTER) == 0) {
        *token = TOKEN_ENTITY;
    } else {
        print_err(MSG_UNRECOGNIZED_TOKEN);
        exit(EXIT_UNRECOGNIZED_TOKEN);
    }
}

void get_delimiter(char* token, char* delimiter) {
    if (strcmp(token, TOKEN_ENTITY) == 0) {
        *delimiter = TOKEN_ENTITY_DELIMITER_VALUE;
    } else if (strcmp(token, TOKEN_ENTITY_TYPE) == 0) {
        *delimiter = TOKEN_ENTITY_TYPE_DELIMITER_VALUE;
    } else if (strcmp(token, TOKEN_FILTER) == 0) {
        *delimiter = TOKEN_FILTER_DELIMITER_VALUE;
    } else {
        print_err(MSG_UNRECOGNIZED_TOKEN);
        exit(EXIT_UNRECOGNIZED_TOKEN);
    }
}

void validate(char* token, char value[], int length) {
    if (strcmp(token, TOKEN_ENTITY) == 0) {
        validate_entity(value, length);
    } else if (strcmp(token, TOKEN_ENTITY_TYPE) == 0) {
        validate_entity_type(value, length);
    } else if (strcmp(token, TOKEN_FILTER) == 0) {
        validate_filter(value, length);
    } else {
        print_err(MSG_UNRECOGNIZED_TOKEN);
        exit(EXIT_UNRECOGNIZED_TOKEN);
    }
}

void validate_entity(char value[], int length) {
    int i = 0;
    char msg[100];
    
    for (; i<length; i++) {
        switch (value[i]) {
            case '&' :
            case '=' :
            case ',' :
            case '+' :
            case '/' :
            case '\\' :
            case '*' :
            sprintf(msg, "Invalid character '%c' in Entity '%s'", value[i], value);
            print_err(msg);
            exit(EXIT_INVALID_CHARACTER);
        }
    }
}

void validate_entity_type(char value[], int length) {
    int ok = 0;
    char msg[100];
    
    if (strcmp(value, "String") == 0 ) {
        ok = 1;
    }
    
    if (strcmp(value, "Int") == 0 ) {
        ok = 1;
    }
    
    if (strcmp(value, "Double") == 0 ) {
        ok = 1;
    }
    
    if (strcmp(value, "()") == 0 ) {
        ok = 1;
    }
    
    if (strcmp(value, "!()") == 0 ) {
        ok = 1;
    }
    
    if (strcmp(value, "?()") == 0 ) {
        ok = 1;
    }
    
    if (ok == 0) {
        sprintf(msg, "Invalid type '%s'", value);
        print_err(msg);
        exit(EXIT_INVALID_CHARACTER);
    }
}


void validate_filter(char value[], int length) {
    // Since we can have different types of filters, we need to first figure
    // out what type of filter this is.
    
    if (value[0] == '\'' || value[0] == '"') {
        validate_quoted_string(value, length);
    }
    
    if (value[0] == '(' || value[0] == '^' || value[0] == '!') {
        validate_list(value, length);
    }
}

void validate_quoted_string(char value[], int length) {
    // Let's make sure that there is an ending quote
    int i = 0;
    int ok = 0;
    char msg[100];
        
    for (; i<length-1; i++) {
        // i+1 because would be comparing the first quote to the first quote
        if (value[i+1] == value[0]) {
            ok = 1;
        }
    }
        
    if (ok == 0) {
        sprintf(msg, "You forgot to close your string with a '%c'", value[0]);
        print_err(msg);
        exit(EXIT_INVALID_CHARACTER);
    }
}

void validate_list(char value[], int length) {
    int i = 0;
    int ok = 0;
    char msg[100];
    int x = 1; // Start at one to skip '('
    char buffer[100];
    
    // List must have at least two items
    if (total_list_items(value, length) <= 1) {
        sprintf(msg, "%s: List must have at least two items.", token_array[nodeIndex-1]->value);
        print_err(msg);
        exit(EXIT_INVALID_CHARACTER);
    }
    
    // If we have a '^' or a '!', we need to make sure it's preceded by a '(' 
    if (value[0] == '^' || value[0] == '!') {
        if (value[1] != '(') {
            sprintf(msg, "Expecting a '(', but got a '%c'", value[1]);
            print_err(msg);
            exit(EXIT_INVALID_CHARACTER);
        }
    }
    
    // List must end with a ')'
    if (value[length-1] != ')') {
        sprintf(msg, "Expecting a ')', but got a '%c'", value[length-1]);
        print_err(msg);
        exit(EXIT_INVALID_CHARACTER);
    }
    
    // Extract each item and validate it
    for (; x<length; x++) {
        if (value[x] != ',' && value[x] != ')') {
            buffer[i] = value[x];
            i++;
        } else {
            buffer[i] = '\0';
            
            // Int
            if (strcmp(token_array[nodeIndex-2]->value, "Int") == 0) {
                ok = validate_int(buffer, i);
                
                if (ok == 0) {
                    sprintf(msg, "This is not an integer: %s.", buffer);
                    print_err(msg);
                    exit(EXIT_INVALID_CHARACTER);
                }
            // Double
            } else if (strcmp(token_array[nodeIndex-2]->value, "Double") == 0) {
                ok = validate_double(buffer, i);
                
                if (ok == 1) {
                    sprintf(msg, "This is not an double: %s.", buffer);
                    print_err(msg);
                    exit(EXIT_INVALID_CHARACTER);
                } else if (ok == 2) {
                    sprintf(msg, "Too many decimals: %s.", buffer);
                    print_err(msg);
                    exit(EXIT_INVALID_CHARACTER);
                }
            }
            
            i = 0; // Reset buffer index
        }
    }
}

int total_list_items(char value[], int length) {
    int i = 0;
    int x = 0;
    int inside_string = 0;
    char quote_type; // remember starting quote
    
    // If we have a length of two, we either have an empty list: () or a
    // malformed list.
    if (length == 2) {
        return 0;
    }
    
    for (; i<length; i++) {
        if (value[i] == '\'' || value[i] == '"') {
            if (inside_string == 1 && quote_type == value[i]) {
                // We found the ending quote so we are no longer in a string
                inside_string = 0;
            } else {
                // Found a starting quote so we are now in a string
                inside_string = 1;
                quote_type = value[i];
            }
        }
            
        // We have two delimiters: ',' and ')'.
        if ((value[i] == ',' || value[i] == ')') && inside_string == 0) {
            x++;
        }
    }
    
    return x;
}

int validate_int(char value[], int length) {
    int i = 0;
    
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
            default:
                return 0; // Bad character
        }
    }
    
    return 1;
}

/**
 * 0 - Valid Double
 * 1 - Non-numeric value found
 * 2 - Too many decimal points found.
 */
int validate_double(char value[], int length) {
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
            default:
                return 1;
        }
    }
    
    if (dec > 1) {
        return 2;
    }
    
    return 0;
}

/*
 * This function will look for common mistakes
 */
int lookout(char buffer[], int length, char delimiter, char c) {
    int i = 0;
    char msg[100];
    char substr[100] = {};
    
    // we don't want all the buffer garbage
    for (; i<length; i++)
        substr[i] = buffer[i];
    
    if (delimiter == ':') {
        if (c == '=') {
            sprintf(msg, "You must specify a type for '%s'.", substr);
            print_err(msg);
            exit(EXIT_INVALID_CHARACTER);
        }
    }
}

int parse(const char* code) {
    int i = 0;
    int x = 0;
    char buffer[100] = {};
    char* token = TOKEN_ENTITY;
    char delimiter;
    
    get_delimiter(token, &delimiter);
    
    while (code[i] != '\0') {
        // Add to buffer until we reach the delimiter
        if (code[i] != delimiter) {
            lookout(buffer, x, delimiter, code[i]);
            buffer[x++] = code[i];
        } else {
            buffer[x] = '\0';
            
            node* n;
            n = (node*) malloc(sizeof(node));
            
            strcpy(n->name, token);
            strcpy(n->value, buffer);
            
            token_array[nodeIndex] = n;
            nodeIndex++;
            
            validate(token, buffer, x);
            
            x = 0;
            
            get_token(&token);
            get_delimiter(token, &delimiter);
        }  
        
        i++; 
    }
    
    // We need to capture the end of the script.  Right now, I must specify '&' 
    // at the end of the script.
    
    return 0;
}

void print_err(char* msg) {
    printf("ERROR: %s\n", msg);
}

int main(int argc, const char* argv[]) {
    int i = 0;
    
    if (argc <2) {
        print_err(MSG_NO_CODE_TO_PROCESS);
        exit(EXIT_NO_CODE_TO_PROCESS);
    }
    
    parse(argv[1]);
    
    for (; i<nodeIndex; i++) {
        printf("%s - %s\n", token_array[i]->name, token_array[i]->value);
    }
    
    return 0;
}
