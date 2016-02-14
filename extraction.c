#include "extraction.h"

char* __PREFIX_get_resource(char code[]) {
    int found = 0;
    int cursor = 0;

    // Find the delimiter.  This will be an operator.
    while (code[cursor] != '\0') {
        switch (code[cursor]) {
            case '>' :
            case '<' :
            case '!' :
            case '=' :
                found = 1;
                cursor--;
                break;

        }

        if (found == 1) {
            break;
        }

        cursor++;
    }

    char* p_resource = (char*)malloc(RESOURCE_MAX+1);
    __PREFIX_substr(code, 0, cursor, p_resource, RESOURCE_MAX);

    return p_resource;
}

char* __PREFIX_get_operator(char code[]) {
    int start = 0;
    int cursor = 0;

    // Find the operator.
    while (code[cursor] != '\0') {
        switch (code[cursor]) {
            case '>' :
            case '<' :
            case '!' :
                start = cursor;

                if (code[cursor+1] == '=') {
                    // Operator is either a '>=', '<=' or '!='.
                    cursor++;
                }
                break;
            case '=' :
                start = cursor;
                break;

        }

        if (start > 0) {
            break;
        }

        cursor++;
    }

    char* p_operator = (char*)malloc(OPERATOR_MAX+1);

    __PREFIX_substr(code, start, cursor, p_operator, OPERATOR_MAX);

    return p_operator;
}

char* __PREFIX_get_filter(char code[]) {
    int start = 0;
    int cursor = 0;
    int inside_string = 0;
    char quote_type; // remember starting quote

    while (code[cursor] != '\0') {
        // We only want to find the staring point once.
        if (start == 0) {
            switch (code[cursor]) {
                case '>' :
                case '<' :
                case '!' :
                case '=' :
                    // We found the operator
                    if (code[cursor+1] == '=') {
                        // Operator is either a '>=', '<=' or '!='.
                        start = cursor+2;
                    } else {
                        start = cursor+1;
                    }

                    break;
            }
        }

        if ((code[cursor] == '&' || code[cursor] == '|') && inside_string == 0) {
            // Filter ends
            cursor--;
            break;
        }

        if (code[cursor] == '\'' || code[cursor] == '"') {
           if (inside_string == 1 && quote_type == code[cursor]) {
               // We found the ending quote so we are no longer in a string
               inside_string = 0;
           } else {
               // Found a starting quote so we are now in a string
               inside_string = 1;
               quote_type = code[cursor];
           }
       }

       cursor++;
    }

    char* p_filter = (char*)malloc(FILTER_MAX+1);

    __PREFIX_substr(code, start, cursor, p_filter, FILTER_MAX);

    return p_filter;
}

char* __PREFIX_get_conjunctive(char code[]) {
    char* p_conjunctive = (char*)malloc(OPERATOR_MAX+1);
    int length = strlen(code);

    __PREFIX_substr(code, length-1, length-1, p_conjunctive, OPERATOR_MAX);

    return p_conjunctive;
}

// We try to infer the type.
int __PREFIX_get_filter_type(char code[]) {
    if (code[0] == '@') {
        return FILTER_TYPE_VAR;
    } else if (code[0] == '(') {
        return FILTER_TYPE_LIST;
    } else if (code[0] == '\'' || code[0] == '"') {
        return FILTER_TYPE_STRING;
    } else if (strcmp(code, "NULL") == 0 || strcmp(code, "null") == 0) {
        return FILTER_TYPE_NULL;
    } else {
        // Some sort of number I guess
        int i = 0;
        while (code[i] != '\0') {
            if (code[i] == '.') {
                return FILTER_TYPE_DOUBLE;
            }

            i++;
        }

        return FILTER_TYPE_INT;
    }
}
