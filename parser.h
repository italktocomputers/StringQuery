#define RESOURCE_MAX 100
#define RESOURCE_MAX_MSG "Resource too large!  Aborting..."

#define RESOURCE_TYPE_MAX 100
#define RESOURCE_TYPE_MAX_MSG "Resource too large!  Aborting..."

#define OPERATOR_MAX 2
#define OPERATOR_MAX_MSG "Operator too large!  Aborting..."

#define FILTER_MAX 1000
#define FILTER_MAX_MSG "Filter too large!  Aborting..."

#define CONJUNCTIVE_MAX 1
#define CONJUNCTIVE_MAX_MSG "Conjunctive operator too large!  Aborting..."

#define MAX_RESOURCES 100

#define DEFAULT_EXIT 1


struct Statement {
    char* resource;
    char* type;
    char* operator;
    char* filter;
    char* filter_type;
    char* conjunctive;
};
