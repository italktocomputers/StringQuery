static char* listToSQL(Statement* st, char* sql) {
    int i = 1;
    int x = 0;
    char newstr[100] = {};
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
    char sql[1000] = {};
    
    for (; i<sts_index; i++) {
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
