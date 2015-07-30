#define ENTITY_MAX 100
#define ENTITY_MAX_MSG "Entity too large!  Aborting..."

#define ENTITY_TYPE_MAX 100
#define ENTITY_TYPE_MAX_MSG "Entity too large!  Aborting..."

#define OPERATOR_MAX 2
#define OPERATOR_MAX_MSG "Operator too large!  Aborting..."

#define FILTER_MAX 1000
#define FILTER_MAX_MSG "Filter too large!  Aborting..."

#define LOGIC_OP_MAX 1
#define LOGIC_OP_MAX_MSG "Logic operator too large!  Aborting..."

#define DEFAULT_EXIT 1


typedef struct Statement {
    char* entity;
    char* type;
    char* operator;
    char* filter;
    char* filter_type;
    char* logic;
} Statement;
