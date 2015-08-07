static void toJSON(struct Statement* sts[], int sts_index) {
    int i = 0;
    
    printf("[\n");
    
    for (; i<sts_index; i++) {
        printf(" {\n");
        printf("  'Resource': '%s',\n", sts[i]->resource);
        printf("  'Type': '%s',\n", sts[i]->type);
        printf("  'Operator': '%s',\n", sts[i]->operator);
        
        if (strcmp(sts[i]->filter_type, "Scalar") == 0 && strcmp(sts[i]->type, "String") == 0) {
            // We are dealing with a string so we don't want to quote the JSON
            // property.  If we did, we would have double quoting.
            printf("  'Filter': %s,\n", sts[i]->filter);
        } else if (strcmp(sts[i]->filter_type, "List") == 0) {
            // This is a List so we want to replace '(' and ')' with 
            // '[' and ']' so it becomes a JSON array.
            int x = 1;
            char newfilter[100] = {};
            int length = strlen(sts[i]->filter);
            
            // Replace '(' with '['
            newfilter[0] = '[';
            
            // Copy rest of filter minus '(' and ')'
            for (; x<length-1; x++) {
                newfilter[x] = sts[i]->filter[x];
            }
            
            // Replace ')' with ']'
            newfilter[length-1] = ']';
            
            printf("  'Filter': %s,\n", newfilter);
        } else {
            // Int or Double
            printf("  'Filter': %s,\n", sts[i]->filter);
        }
        
        printf("  'Filter_Type': '%s'\n", sts[i]->filter_type);
        printf("  'Conjunctive': '%s'\n", sts[i]->conjunctive);
        printf(" }");
        
         if (i < sts_index-1)
           printf(",");
           
        printf("\n");
    }
    
    printf("];\n");
}
