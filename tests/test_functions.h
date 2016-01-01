#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H
    #define NUM_SPACES_TO_RIGHT 50
    
    void print_dots(int l);
    void pass(char* test_name);
    void fail(char* test_name, char* expected, char* actual);
    void test_string(char* test_name, char* expected, char* actual);
    void test_int(char* test_name, int expected, int actual);
    
#endif
