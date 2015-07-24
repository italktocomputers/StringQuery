/*
 * Copyright (c) 2015, Andrew Schools <andrewschools@me.com>
 * Permission is hereby granted, free of charge, to any
 * person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the
 * Software without restriction, including without
 * limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the
 * following conditions:
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 *
 * ########################
 * ### MezeyQuery v 0.1 ###
 * ########################
 *
 *
 * Valid syntax: 
 *     Entity:DataType=Filter[&Entity=Filter&...]
 * 
 * Entity is a string, specifying the resource to apply the filter against.
 * 
 * Type is a string, specifying the data type of the resource.  If the entity is
 * a type string, then the filter must be a type string or a list which are
 * composed of strings.
 * 
 * Filter values can be any of the following data types:
 *    - String
 *    - Int
 *    - Double
 *    - List: (Entity must equal ALL values in list)
 *    - List! (Entity must NOT equal any value from list)
 *    - List? (Entity must equal at least one value from list)
 *
 * Valid List syntax:
 *     List(value1[,value2,...]
 * 
 * Supported operators:
 *    - : (Equal to)
 *    - > (Greater than)
 *    - < (Less than)
 *    - >: (Greater than or equal to)
 *    - <: (Less than or equal to)
 *    - :! (Not equal to)
 *    
 *
 * Examples:
 *     Get all records that have a FirstName equal to 'Andrew' or 'Jeff', who's 
 *     Age is greater than or equal to 20
 *      
 *     FirstName:String=list?('Andrew Schools', 'Jeff')&Age:Int=>:20 
 *
 *  
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

#define TOKEN_ENTITY_DELIMITER_VALUE ':'
#define TOKEN_ENTITY_TYPE_DELIMITER_VALUE '='
#define TOKEN_FILTER_DELIMITER_VALUE '&'
#define TOKEN_FILTER_ITEM_DELIMITER_VALUE ','

int parse(const char*);
void get_token(char**);
void print_err(char*);
void get_delimiter(char*, char*);
void validate(char*, char[], int);
void validate_entity(char[], int);
void validate_entity_type(char[], int);
void validate_assignment(char[], int);
void validate_datatype(char[], int);
void validate_entity_delimiter_and(char[], int);

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
        validate_datatype(value, length);
    } else if (strcmp(token, TOKEN_ASSIGNMENT) == 0) {
        validate_assignment(value, length);
    } else if (strcmp(token, TOKEN_FILTER_DELIMITER) == 0) {
        validate_entity_delimiter_and(value, length);
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

}

void validate_assignment(char value[], int length) {

}


void validate_datatype(char value[], int length) {

}

void validate_entity_delimiter_and(char value[], int length) {

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
            buffer[x++] = code[i];
        } else {
            buffer[x] = '\0';
            
            validate(token, buffer, x);
            
            node* n;
            n = (node*) malloc(sizeof(node));
            
            strcpy(n->name, token);
            strcpy(n->value, buffer);
            
            token_array[nodeIndex] = n;
            nodeIndex++;
            
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
