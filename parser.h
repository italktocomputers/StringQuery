#include <stdlib.h>

#ifndef PARSER_H
#define PARSER_H

    struct Statement {
        char* resource;
        char* type;
        char* operator;
        char* filter;
        char* filter_type;
        char* conjunctive;
    };
     
    static int parse(char[], int*, struct Statement*[], int*);
    static int get_statement(char[], int*, struct Statement*[], int*);
    static int is_end(char[], int*);
    static int substr(char*, int, int, char*, int);
    static int clean(char[], int, char*);

    int hex_to_dec(char[], int);

    static void urldecode(char[], char*);
    static void print_error(char[], int, char[], int);
    static void exception(char[], char[], int);

    char* get_resource(char[], int*);
    char* get_resource_type(char[], int*);
    char* get_operator(char[], int*);
    char* get_filter(char[], int*, char*);
    char* get_conjunctive(char[], int*);

    static void validate_resource(char[], int, char[], int, const struct Statement*);
    static void validate_resource_type(char[], int, char[], int, const struct Statement*);
    static void validate_operator(char[], int, char[], int, const struct Statement*);
    static void validate_filter(char[], int, char*, char[], int, const struct Statement*);
    static void validate_conjunctive(char[], int, char[], int, const struct Statement*);

    static void validate_var(char[], int, char[], int, const struct Statement*);
    static void validate_string(char[], int, char[], int, const struct Statement*);
    static void validate_int(char[], int, char[], int, const struct Statement*);
    static void validate_double(char[], int, char[], int, const struct Statement*);
    static void validate_list(char[], int, char*, char[], int, const struct Statement*);
    static void validate_list_string(char[], int, char[], int, const struct Statement*);
    static void validate_list_double(char[], int, char[], int, const struct Statement*);
    static void validate_list_int(char[], int, char[], int, const struct Statement*);
     
#endif
