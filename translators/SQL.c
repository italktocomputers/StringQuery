#define MAX_SQL 60000

static void buffer_checks(Statement* st) {
    if (strlen(st->entity) >= ENTITY_MAX) {
        printf(ENTITY_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
    
    if (strlen(st->type) >= ENTITY_TYPE_MAX) {
        printf(ENTITY_TYPE_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
    
    
    if (strlen(st->operator) >= OPERATOR_MAX) {
        printf(OPERATOR_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
    
    if (strlen(st->filter) >= FILTER_MAX) {
        printf(FILTER_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
    
    if (strlen(st->logic) >= LOGIC_OP_MAX) {
        printf(LOGIC_OP_MAX_MSG);
        exit(DEFAULT_EXIT);
    }
}

static char* listToSQL(Statement* st, char* sql) {
    int i = 1;
    int x = 0;
    char newstr[FILTER_MAX] = {};
    int length = strlen(st->filter);
    
    // We need to remove '(' and ')' characters
    for (; i<length-1; i++) {
        if (st->filter[i] == ' ')
            continue; // skip spaces
            
        newstr[x++] = st->filter[i];
    }
    
    char* token;
    token = strtok(newstr, ",");
    
    sprintf(sql + strlen(sql), "%s %s %s", st->entity, st->operator, token);
    
    while (token != NULL) {
        int l = strlen(token);
        token = strtok (NULL, ",");
        
        if (token != NULL) {
            sprintf(sql + strlen(sql), " OR %s %s %s", st->entity, st->operator, token);
        }
    }
}

void toSQL(Statement* sts[], int sts_index) {
    int i = 0;
    char sql[MAX_SQL] = {};
    
    for (; i<sts_index; i++) {
        buffer_checks(sts[i]);
        
        if (strcmp(sts[i]->filter_type, "Scalar") == 0) {
            if (i > 0) {
                if (strcmp(sts[i-1]->logic, "&") == 0) {
                    strcat(sql, " AND ");
                } else {
                    strcat(sql, " OR ");
                }
            }
            
            sprintf(sql + strlen(sql), "%s %s %s", sts[i]->entity, sts[i]->operator, sts[i]->filter);
        } else {
            // We need to unpack the list
            listToSQL(sts[i], sql);
        }
    }
    
    printf("SQL: %s\n", sql);
}
