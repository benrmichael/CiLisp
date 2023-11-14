#ifndef __cilisp_h_
#define __cilisp_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>


#define NAN_RET_VAL (RET_VAL){DOUBLE_TYPE, NAN}
#define ZERO_RET_VAL (RET_VAL){INT_TYPE, 0}


#define BISON_FLEX_LOG_PATH "bison_flex.log"
FILE* read_target;
FILE* flex_bison_log_file;
size_t yyreadline(char **lineptr, size_t *n, FILE *stream, size_t n_terminate);


int yyparse(void);
int yylex(void);
void yyerror(char *, ...);
void warning(char*, ...);


typedef enum {
    START_STATE,
    INT_STATE,
    FLOAT_STATE,
    INVALID_STATE,
} STATE;

typedef enum func_type {
    NEG_FUNC,       // 0
    ABS_FUNC,       // 1
    EXP_FUNC,       // 2
    EXP2_FUNC,      // 3
    LOG_FUNC,       // 4
    SQRT_FUNC,      // 5
    CBRT_FUNC,      // 6
    PRINT_FUNC,     // 7
    // --------------- END UNARY FUNCS
    SUB_FUNC,       // 8
    DIV_FUNC,       // 9
    REMAINDER_FUNC, // 10
    POW_FUNC,       // 11
    EQUAL_FUNC,     // 12
    LESS_FUNC,      // 13
    GREATER_FUNC,   // 14
    // --------------- END BINARY FUNCS
    MULT_FUNC,      // 15
    ADD_FUNC,       // 16
    HYPOT_FUNC,     // 17
    MAX_FUNC,       // 18
    MIN_FUNC,       // 19
    // --------------- END N-ARY FUNCS
    RAND_FUNC,      // 20
    READ_FUNC,      // 21
    // --------------- END NO ARG FUNCS
    CUSTOM_FUNC     // 22
} FUNC_TYPE;

typedef enum num_type {
    INT_TYPE,
    DOUBLE_TYPE,
    NO_TYPE
} NUM_TYPE;

typedef enum {
    VAR_TYPE,
    LAMBDA_TYPE,
    ARG_TYPE,
} SYMBOL_TYPE;

typedef struct {
    NUM_TYPE type;
    double value;
} AST_NUMBER;

typedef AST_NUMBER RET_VAL;

typedef struct ast_num_list{
    AST_NUMBER number;
    struct ast_num_list *next;
} AST_NUM_LIST;

typedef struct ast_function {
    FUNC_TYPE func;
    char *func_id;
    struct ast_node *opList;
} AST_FUNCTION;


typedef enum ast_node_type {
    NUM_NODE_TYPE,  // 0
    FUNC_NODE_TYPE, // 1
    SYM_NODE_TYPE,  // 2
    SCOPE_NODE_TYPE,// 3
    COND_NODE_TYPE, // 4
} AST_NODE_TYPE;


typedef struct {
    char *id;
} AST_SYMBOL;


typedef struct {
    struct ast_node *child;
} AST_SCOPE;

typedef struct ast_condition_node {
    struct ast_node *op1;
    struct ast_node *op2;
    struct ast_node *op3;
} AST_COND;

typedef struct ast_node {
    AST_NODE_TYPE type;
    struct ast_node *parent;
    struct symbol_table_node *symbolTable;
    union {
        AST_NUMBER number;
        AST_FUNCTION function;
        AST_SYMBOL symbol;
        AST_SCOPE scope;
        AST_COND cond;
    } data;
    struct ast_node *next;
} AST_NODE;


typedef struct symbol_table_node {
    char *id;
    NUM_TYPE type;
    SYMBOL_TYPE symbolType;
    AST_NODE *value;
    struct stack_node *stack;
    struct symbol_table_node *argTable;
    struct symbol_table_node *next;
} SYMBOL_TABLE_NODE;

typedef struct stack_node {
    RET_VAL value;
    struct stack_node *next;
} STACK_NODE;

// Production rule functions
AST_NODE *createNode(AST_NODE_TYPE);
AST_NODE *createNumberNode(double, NUM_TYPE);
AST_NODE *createFunctionNode(char*, FUNC_TYPE, AST_NODE*);
AST_NODE *createSymbolNode(char*);
AST_NODE *createScopeNode(SYMBOL_TABLE_NODE*, AST_NODE*);
AST_NODE *createConditionNode(AST_NODE*, AST_NODE*, AST_NODE*);

SYMBOL_TABLE_NODE *createSymbolTableNode();
SYMBOL_TABLE_NODE *createLetElem(SYMBOL_TYPE, NUM_TYPE, char*, AST_NODE*, SYMBOL_TABLE_NODE*);
SYMBOL_TABLE_NODE *createLetList(SYMBOL_TABLE_NODE*, SYMBOL_TABLE_NODE*);
SYMBOL_TABLE_NODE *createArg(char*);
SYMBOL_TABLE_NODE *createArgList(char*, SYMBOL_TABLE_NODE*);
SYMBOL_TABLE_NODE *findLambdaFunction(AST_NODE*);

void evalFunctionPush(AST_NODE*, SYMBOL_TABLE_NODE*);
void evalFunctionPop(SYMBOL_TABLE_NODE*);


AST_NODE *addExpressionToList(AST_NODE*, AST_NODE*);

FUNC_TYPE resolveFunc(char *);

// Eval functions
RET_VAL eval(AST_NODE *node);
RET_VAL evalFuncNode(AST_NODE*);
RET_VAL evalScopeNode(AST_NODE*);
RET_VAL evalCondNode(AST_NODE*);
RET_VAL evalCustomFunc(AST_NODE*);
RET_VAL evalTypeCasting(RET_VAL, NUM_TYPE);
RET_VAL evalUnaryFuncs(AST_NODE*, FUNC_TYPE);
RET_VAL evalBinaryFuncs(AST_NODE*, FUNC_TYPE);
RET_VAL evalNaryFuncs(AST_NODE*, FUNC_TYPE);
RET_VAL evalNoArgFuncs(AST_NODE*, FUNC_TYPE);

// Print function
void printRetVal(RET_VAL val);

// Free functions
void freeNode(AST_NODE *node);
void freeAstNumList(AST_NUM_LIST*);
void freeFunctionNode(AST_NODE*);
void freeSymbolTable(SYMBOL_TABLE_NODE*);

// Func array declarations
RET_VAL (*unaryFuncs[])(AST_NUMBER);
RET_VAL (*binaryFuncs[])(AST_NUMBER, AST_NUMBER);
RET_VAL (*naryFuncs[])(AST_NUM_LIST*);

// All of the unary functions
RET_VAL evalNegFunc(AST_NUMBER);
RET_VAL evalAbsFunc(AST_NUMBER);
RET_VAL evalExpFunc(AST_NUMBER);
RET_VAL evalExp2Func(AST_NUMBER);
RET_VAL evalLogFunc(AST_NUMBER);
RET_VAL evalSqrtFunc(AST_NUMBER);
RET_VAL evalCbrtFunc(AST_NUMBER);
RET_VAL evalPrintFunc(AST_NUMBER);

// All of the binary functions
RET_VAL evalSubFunc(RET_VAL, RET_VAL);
RET_VAL evalDivFunc(RET_VAL, RET_VAL);
RET_VAL evalRemFunc(RET_VAL, RET_VAL);
RET_VAL evalPowFunc(RET_VAL, RET_VAL);
RET_VAL evalEqualFunc(RET_VAL, RET_VAL);
RET_VAL evalLessFunc(RET_VAL, RET_VAL);
RET_VAL evalGreaterFunc(RET_VAL, RET_VAL);


// All of the n-ary functions
RET_VAL evalAddFunc(AST_NUM_LIST*);
RET_VAL evalMultFunc(AST_NUM_LIST*);
RET_VAL evalHypotFunc(AST_NUM_LIST*);
RET_VAL evalMaxFunc(AST_NUM_LIST*);
RET_VAL evalMinFunc(AST_NUM_LIST*);

// No argument functions
RET_VAL evalReadFunc();
RET_VAL evalRandFunc();

#endif
