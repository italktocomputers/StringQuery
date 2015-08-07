#define MAX_SQL 60000

static void buffer_checks(struct Statement* st) {
    if (strlen(st->resource) > RESOURCE_MAX+1) {
        printf(RESOURCE_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
    
    if (strlen(st->type) > RESOURCE_TYPE_MAX+1) {
        printf(RESOURCE_TYPE_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
    
    if (strlen(st->operator) > OPERATOR_MAX+1) {
        printf(OPERATOR_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
    
    if (strlen(st->filter) > FILTER_MAX+1) {
        printf(FILTER_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
    
    if (st->conjunctive != NULL) {
        if (strlen(st->conjunctive) > CONJUNCTIVE_MAX+1) {
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

void toSQL(struct Statement* sts[], int sts_index) {
    int i = 0;
    char sql[MAX_SQL] = {};
    
    for (; i<sts_index; i++) {
        buffer_checks(sts[i]);
        
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
