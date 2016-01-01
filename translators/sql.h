#include "../parser.h"

#ifndef SQL_H
#define SQL_H

    void buffer_checks(struct Statement* st);
    char* listToSQL(struct Statement* st, char* sql);
    void get_resource_name(char resource[RESOURCE_MAX], char* id);
    int in_array(char* arr[], char item[], int length);
    int unique(char* resources[], char* unique_resources[MAX_RESOURCES], int length);
    void toSQL(struct Statement* sts[], int sts_index);
    
#endif
