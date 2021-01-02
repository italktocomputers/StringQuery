#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "library.h"
#include "extraction.h"

char **__PREFIX_get_statements(char code[], int *k) {
  int i = 0;
  int x = 0;
  int cursor = 0;
  int inside_string = 0;
  char quote_type; // remember starting quote
  char **statements = (char **) malloc(sizeof(char *) * 10);

  char buffer[STATEMENT_MAX_LENGTH];

  // Find the delimiter.  This will be an operator.
  while (1) {
    if (code[cursor] == '\'' || code[cursor] == '"') {
      if (inside_string == 1 && quote_type == code[cursor]) {
        // We found the ending quote so we are no longer in a string
        inside_string = 0;
      }
      else {
        // Found a starting quote so we are now in a string
        inside_string = 1;
        quote_type = code[cursor];
      }
    }

    if ((code[cursor] == '&' || code[cursor] == '|' || code[cursor] == '\0') && inside_string == 0) {
      // Statement found so move from character buffer to array.

      if (code[cursor] == '\0') {
        buffer[i] = '&';
      }
      else {
        buffer[i] = code[cursor];
      }

      buffer[++i] = '\0';

      char *p = (char *) malloc(i + 1);
      strcpy(p, buffer);

      statements[x++] = p;

      i = 0;
    }
    else {
      // Add character to character buffer.
      buffer[i++] = code[cursor];
    }

    if (code[cursor] == '\0')
      break;

    cursor++;
  }

  *k = x;
  return statements;
}

//
// Each extraction function except for the get_statements function works on a per
// statement basis.
//

char *__PREFIX_get_resource(char code[]) {
  int cursor = 0;

  // Find the delimiter.  This will be an operator.
  while (code[cursor] != '\0') {
    if (code[cursor] == ':') {
      cursor--;
      break;
    }

    cursor++;
  }

  char *p_resource = (char *) malloc(RESOURCE_MAX + 1);
  __PREFIX_substr(code, 0, cursor, p_resource, RESOURCE_MAX);

  return p_resource;
}

char *__PREFIX_get_resource_type(char code[]) {
  int found = 0;
  int cursor = 0;
  int start = 0;

  // Find the delimiters.
  while (code[cursor] != '\0') {
    switch (code[cursor]) {
      case ':' :
        start = cursor + 1;
        break;
      case '>' :
      case '<' :
      case '!' :
      case '=' :
        found = 1;
        cursor--;
        break;

    }

    if (found == 1)
      break;

    cursor++;
  }

  char *p_resource_type = (char *) malloc(RESOURCE_TYPE_MAX + 1);
  __PREFIX_substr(code, start, cursor, p_resource_type, RESOURCE_TYPE_MAX);

  return p_resource_type;
}

char *__PREFIX_get_operator(char code[]) {
  int start = 0;
  int cursor = 0;

  // Find the operator.
  while (code[cursor] != '\0') {
    switch (code[cursor]) {
      case '>' :
      case '<' :
      case '!' :
        start = cursor;

        if (code[cursor + 1] == '=') {
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

  char *p_operator = (char *) malloc(OPERATOR_MAX + 1);

  __PREFIX_substr(code, start, cursor, p_operator, OPERATOR_MAX);

  return p_operator;
}

char *__PREFIX_get_filter(char code[]) {
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
          if (code[cursor + 1] == '=') {
            // Operator is either a '>=', '<=' or '!='.
            start = cursor + 2;
          }
          else {
            start = cursor + 1;
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
      }
      else {
        // Found a starting quote so we are now in a string
        inside_string = 1;
        quote_type = code[cursor];
      }
    }

    cursor++;
  }

  char *p_filter = (char *) malloc(FILTER_MAX + 1);

  __PREFIX_substr(code, start, cursor, p_filter, FILTER_MAX);

  return p_filter;
}

char *__PREFIX_get_conjunctive(char code[]) {
  char *p_conjunctive = (char *) malloc(OPERATOR_MAX + 1);
  int length = strlen(code);

  __PREFIX_substr(code, length - 1, length - 1, p_conjunctive, OPERATOR_MAX);

  return p_conjunctive;
}

char **__PREFIX_expand_list(char code[]) {
  int i = 0;
  int x = 0;
  int cursor = 0;
  int count = 1;
  int inside_string = 0;
  char quote_type; // remember starting quote
  char buffer[100];

  // Run through the list once to see how many items we need to collect.
  while (1) {
    if (code[cursor] == ',' && inside_string == 0)
      count++;

    __PREFIX_inside_string(&inside_string, &quote_type, code[cursor]);

    if (code[cursor] == '\0')
      break;

    cursor++;
  }

  char **list = (char **) malloc(sizeof(char *) * count);
  inside_string = 0; // Reset from above
  cursor = 0; // Reset from above

  while (1) {
    __PREFIX_inside_string(&inside_string, &quote_type, code[cursor]);

    if ((code[cursor] == ',' && inside_string == 0) || code[cursor] == '\0') {
      buffer[i] = '\0';

      char *tmp = (char *) malloc(i + 1);
      strcpy(tmp, buffer);

      list[x++] = tmp;

      i = 0;
    }
    else {
      buffer[i++] = code[cursor];
    }

    if (code[cursor] == '\0')
      break;

    cursor++;
  }

  return list;
}
