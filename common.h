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

#define STATEMENTS_ARRAY_LENGTH 1000
#define CODE_BUFFER_LENGTH 50000
#define STATEMENT_MAX_LENGTH 5000
#define LIST_ITEM_MAX_LENGTH 100

#define MSG_NO_CODE_TO_PROCESS "No code to process"
#define MSG_TOO_MUCH_CODE_TO_PROCESS "Too much code to process"

#define EXIT_INVALID_SYNTAX 4

#define MSG_NO_EXPORT_TYPE "You have not specified a export type"
#define MSG_NO_EXPORT_SUPPORT "Supported export formats: JSON, SQL"
#define MSG_NO_CODE_SWITCH "Must specify --code or --file"
#define MSG_NO_CODE_FORMAT "You have not specified a code format"

#define EXPORT_ARG_LENGTH 10
#define CODE_FORMAT_ARG_LENGTH 50

#define RESOURCE_MAX 100
#define RESOURCE_MAX_MSG "Resource too large!  Aborting..."

#define RESOURCE_TYPE_MAX 10
#define RESOURCE_TYPE_MAX_MSG "Resource type too large!  Aborting..."

#define OPERATOR_MAX 2
#define OPERATOR_MAX_MSG "Operator too large!  Aborting..."

#define FILTER_MAX 1000
#define FILTER_MAX_MSG "Filter too large!  Aborting..."

#define CONJUNCTIVE_MAX 1
#define CONJUNCTIVE_MAX_MSG "Conjunctive operator too large!  Aborting..."

#define MAX_RESOURCES 100

#define DEFAULT_EXIT 1

#define __PREFIX sQ

#define NO_ERROR 0
#define ERROR_INVALID_CHR 100
#define ERROR_UNEXPECTED_END_OF_LINE 101
#define ERROR_INVALID_RESOURCE 102
#define ERROR_INVALID_OPERATOR 103
#define ERROR_INVALID_CONJUNCTIVE 104
#define ERROR_VAR_MUST_START_WITH_IDENTIFIER 105
#define ERROR_INVALID_VAR 106
#define ERROR_INVALID_STRING 107
#define ERROR_INVALID_STRING_NO_START 108
#define ERROR_INVALID_STRING_NO_END 109
#define ERROR_INVALID_LIST 110
#define ERROR_INVALID_LIST_NO_START 111
#define ERROR_INVALID_LIST_NO_END 112
#define ERROR_INVALID_INT 113
#define ERROR_INVALID_DOUBLE 114
#define ERROR_INVALID_YEAR 115
#define ERROR_INVALID_MONTH 116
#define ERROR_INVALID_DAY 117
#define ERROR_INVALID_HOUR 118
#define ERROR_INVALID_MINUTE 119
#define ERROR_INVALID_SECOND 120
#define ERROR_INVALID_DATE 121
#define ERROR_INVALID_TIME 122
#define ERROR_INVALID_RESOURCE_TYPE 123
