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
#include <math.h>
#include "parser.h"
#include "library.h"
#include "translators/json.h"
#include "translators/sql.h"

int main(int argc, const char* argv[]) {
    int i = 0;
    int sts_index = 0;
    int cursor = 0;
    char code[CODE_BUFFER_LENGTH] = {};
    char export_type[10] = "JSON";
    int code_switch = 0;
    int file_switch = 0;
    int export_switch = 0;
    int code_format_switch = 0;
    char code_format[50];

    struct Statement* sts[STATEMENTS_ARRAY_LENGTH];

    for (i=1; i<argc; i++) {
        if (strcmp(argv[i], "--code") == 0) {
            code_switch = 1;

            if (argv[i+1] == NULL) {
                printf("ERROR: %s\n", MSG_NO_CODE_TO_PROCESS);
                exit(DEFAULT_EXIT);
            }

            int x = strlen(argv[i+1]);

            if (x >= CODE_BUFFER_LENGTH) {
                printf("ERROR: %s\n", MSG_TOO_MUCH_CODE_TO_PROCESS);
                exit(DEFAULT_EXIT);
            }

            strncpy(code, argv[i+1], CODE_BUFFER_LENGTH);
        } else if (strcmp(argv[i], "--file") == 0) {
            file_switch = 1;

            if (argv[i+1] == NULL) {
                printf("ERROR: %s\n", MSG_NO_CODE_TO_PROCESS);
                exit(DEFAULT_EXIT);
            } else {
                FILE *fp;
                char c;
                int x = 0;

                fp = fopen(argv[i+1], "r");

                while ((c = fgetc(fp)) != EOF) {
                    if (x >= CODE_BUFFER_LENGTH) {
                        printf("ERROR: %s\n", MSG_TOO_MUCH_CODE_TO_PROCESS);
                        exit(DEFAULT_EXIT);
                    }

                    code[x++] = c;
                }
            }
        } else if (strcmp(argv[i], "--export") == 0) {
            export_switch = 1;

            if (argv[i+1] == NULL) {
                printf("ERROR: %s\n", MSG_NO_EXPORT_TYPE);
                exit(DEFAULT_EXIT);
            }

            strncpy(export_type, argv[i+1], EXPORT_ARG_LENGTH);
        } else if (strcmp(argv[i], "--code-format") == 0) {
            code_format_switch = 1;

            if (argv[i+1] == NULL) {
                printf("ERROR: %s\n", MSG_NO_CODE_FORMAT);
                exit(DEFAULT_EXIT);
            }

            strncpy(code_format, argv[i+1], CODE_FORMAT_ARG_LENGTH);
        }
    }

    if (code_switch == 0 && file_switch == 0) {
        printf("ERROR: %s\n", MSG_NO_CODE_SWITCH);
        exit(DEFAULT_EXIT);
    }

    char decoded_code[CODE_BUFFER_LENGTH] = {};

    if (code_format_switch == 1 && strcmp(code_format, "urlencoded") == 0) {
        __PREFIX_urldecode(code, decoded_code);
    } else {
        strncpy(decoded_code, code, CODE_BUFFER_LENGTH);
    }

    __PREFIX_parse(decoded_code, &cursor, sts, &sts_index);

    if (strcmp(export_type, "JSON") == 0) {
        __PREFIX_toJSON(sts, sts_index);
    } else if(strcmp(export_type, "SQL") == 0) {
        __PREFIX_toSQL(sts, sts_index);
    } else {
        printf("ERROR: %s\n", MSG_NO_EXPORT_SUPPORT);
        exit(DEFAULT_EXIT);
    }

    return 0;
}
