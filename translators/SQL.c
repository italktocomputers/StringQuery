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

#define MAX_SQL 60000

static void buffer_checks(struct Statement* st) {
    if (strlen(st->resource) > RESOURCE_MAX) {
        printf(RESOURCE_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
    
    if (strlen(st->type) > RESOURCE_TYPE_MAX) {
        printf(RESOURCE_TYPE_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
    
    if (strlen(st->operator) > OPERATOR_MAX) {
        printf(OPERATOR_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
    
    if (strlen(st->filter) > FILTER_MAX) {
        printf(FILTER_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
    
    if (st->conjunctive != NULL) {
        if (strlen(st->conjunctive) > CONJUNCTIVE_MAX) {
            printf(CONJUNCTIVE_MAX_MSG);
            exit(DEFAULT_EXIT);
        }
    }
}

static char* listToSQL(struct Statement* st, char* sql) {
    int i = 1;
    int x = 0;
    char newstr[FILTER_MAX+1] = {};
    int length = strlen(st->filter);
    
    // We need to remove '(' and ')' characters
    for (; i<length-1; i++) {
        if (st->filter[i] == ' ')
            continue; // skip spaces
            
        newstr[x++] = st->filter[i];
    }
    
    char* token;
    token = strtok(newstr, ",");
    
    sprintf(sql + strlen(sql), "%s %s %s", st->resource, st->operator, token);
    
    while (token != NULL) {
        int l = strlen(token);
        token = strtok (NULL, ",");
        
        if (token != NULL) {
            sprintf(sql + strlen(sql), " OR %s %s %s", st->resource, st->operator, token);
        }
    }
    
    if (st->conjunctive != NULL) {
        if (strcmp(st->conjunctive, "&") == 0) {
            strcat(sql, " AND ");
        } else {
            strcat(sql, " OR ");
        }
    }
}

static void get_resource_name(char resource[RESOURCE_MAX], char* id) {
    int i = 0;
    int length = strlen(resource);
    char buffer[RESOURCE_MAX+1] = {};
    
    for(; i<length; i++) {
        if (resource[i] == '.')
            break;
            
        buffer[i] = resource[i];
    }
    
    buffer[i] = '\0';
    strcpy(id, buffer);
}

static int in_array(char* arr[], char item[], int length) {
    int i = 0;
    
    for (; i<length; i++) {
        if (strcmp(arr[i], item) == 0) {
            return 1;
        }
    }
    
    return 0;
}

static int unique(char* resources[], char* unique_resources[MAX_RESOURCES], int length) {
    int i = 0;
    int x = 0;
    
    for (; i<length; i++) {
        if (in_array(unique_resources, resources[i], x) == 0) {
            // Add this item to our array
            char* p = (char*)malloc(RESOURCE_MAX+1);
            strcpy(p, resources[i]);
            
            unique_resources[x++] = p;
        }  
    }
    
    return x;
}

void toSQL(struct Statement* sts[], int sts_index) {
    int i = 0;
    char sql[MAX_SQL] = {};
    
    char* identifiers[MAX_RESOURCES];
    int identifiers_length;
    
    char* unique_identifiers[MAX_RESOURCES] = {};
    int unique_identifiers_length;
    
    //for (; i<sts_index; i++) {
        //buffer_checks(sts[i]);
    
    strcat(sql, "SELECT\n");
    
    
    // Fetch ALL resources and then compile a list of unique ones so we
    // can build a SELECT and FROM list.
    for (i=0; i<sts_index; i++) {
        char identifier[RESOURCE_MAX+1] = {};
        
        // If resource is User.FirstName, then after we call the get_resource_name
        // function, identifier will be equal to 'User' 
        get_resource_name(sts[i]->resource, identifier);
        
        char* p_resource = malloc(RESOURCE_MAX+1);
        
        strcpy(p_resource, identifier);
        
        identifiers[i] = p_resource;
    }
    
    identifiers_length = i;
    
    // We need to also look at each variable type as we might have a resource
    // listed there that is NOT listed elsewhere
    for (i=0; i<sts_index; i++) {
        if (strcmp(sts[i]->type, "@") == 0) {
            char identifier[RESOURCE_MAX+1] = {};
            
            // If variable is User.FirstName, then after we call the get_resource_name
            // function, identifier will be equal to 'User' 
            get_resource_name(sts[i]->filter, identifier);
            
            char* p_resource = malloc(RESOURCE_MAX+1);
            
            strcpy(p_resource, identifier);
            
            identifiers[identifiers_length] = p_resource;
            identifiers_length++;
        }
    }
    
    // We only need unique resource names to buold our SELECT and FROM list 
    unique_identifiers_length = unique(identifiers, unique_identifiers, identifiers_length);
    
    // Print SELECT list
    for (i=0; i<unique_identifiers_length; i++) {
        strcat(sql, " ");
        
        if (i != 0)
            strcat(sql, ",");
        
        strcat(sql, unique_identifiers[i]);
        
        strcat(sql, ".*\n");
    }
    
    strcat(sql, "FROM ");
    
    // Print FROM list
    for (i=0; i<unique_identifiers_length; i++) {
        if (i != 0)
            strcat(sql, ", ");
        strcat(sql, unique_identifiers[i]);
    }
    
    strcat(sql, "\nWHERE ");
    
    for (i=0; i<sts_index; i++) {
        if (strcmp(sts[i]->filter_type, "Scalar") == 0) {
            sprintf(sql + strlen(sql), "%s %s %s", sts[i]->resource, sts[i]->operator, sts[i]->filter);
            
            if (sts[i]->conjunctive != NULL) {
                if (strcmp(sts[i]->conjunctive, "&") == 0) {
                    strcat(sql, " AND ");
                } else {
                    strcat(sql, " OR ");
                }
            }
        } else {
            // We need to unpack the list
            listToSQL(sts[i], sql);
        }
    }
    
    printf("%s\n", sql);
}
