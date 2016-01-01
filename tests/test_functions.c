#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_functions.h"

void print_dots(int l) {
    int i=0;
    int n_dots = NUM_SPACES_TO_RIGHT - l;
    
    for (; i<n_dots; i++) {
        printf(".");
    }
}

void pass(char* test_name) {
    int l = strlen(test_name);
    
    printf("%s", test_name);
    printf("\033[0;32m");
    print_dots(l);
    printf("\033[0;32m[Passed!]\033[0m\n");
}

void fail(char* test_name, char* expected, char* actual) {
    int l = strlen(test_name);
    
    printf("%s", test_name);
    printf("\033[0;31m");
    print_dots(l);
    printf("[Failed!]\n");
    printf("Expecting %s but got %s\033[0m\n", expected, actual);
}

void test_string(char* test_name, char* expected, char* actual) {
    if (strcmp(expected, actual) == 0) {
        pass(test_name);
    } else {
        fail(test_name, expected, actual);
    }
}

void test_int(char* test_name, int expected, int actual) {
    if (expected == actual) {
        pass(test_name);
    } else {
        char* expected_string;
        expected_string = (char*)malloc(3);
        
        char* actual_string;
        actual_string = (char*)malloc(3);
        
        sprintf(expected_string, "%i", expected);
        sprintf(actual_string, "%i", actual);
        
        fail(test_name, expected_string, actual_string);
    }
}
