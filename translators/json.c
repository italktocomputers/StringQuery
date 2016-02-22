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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser.h"

void __PREFIX_toJSON(struct Statement* sts[], int sts_index) {
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
