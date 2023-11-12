#include "cilisp.h"

#define RED "\033[31m"
#define RESET_COLOR "\033[0m"

// yyerror:
// Something went so wrong that the whole program should crash.
// You should basically never call this unless an allocation fails.
// (see the "yyerror("Memory allocation failed!")" calls and do the same.
// This is basically printf, but red, with "\nERROR: " prepended, "\n" appended,
// and an "exit(1);" at the end to crash the program.
// It's called "yyerror" instead of "error" so the parser will use it for errors too.
void yyerror(char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 255, format, args);

    printf(RED "\nERROR: %s\nExiting...\n" RESET_COLOR, buffer);
    fflush(stdout);

    va_end(args);
    exit(1);
}

// warning:
// Something went mildly wrong (on the user-input level, probably)
// Let the user know what happened and what you're doing about it.
// Then, move on. No big deal, they can enter more inputs. ¯\_(ツ)_/¯
// You should use this pretty often:
//      too many arguments, let them know and ignore the extra
//      too few arguments, let them know and return NAN
//      invalid arguments, let them know and return NAN
//      many more uses to be added as we progress...
// This is basically printf, but red, and with "\nWARNING: " prepended and "\n" appended.
void warning(char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 255, format, args);

    printf(RED "WARNING: %s\n" RESET_COLOR, buffer);
    fflush(stdout);

    va_end(args);
}

/*
 * Given a string (yytext) return the type
*/
NUM_TYPE resolveType(char *yytext){
    if(strcmp(yytext, "double") == 0){
        return DOUBLE_TYPE;
    } else if (strcmp(yytext, "int") == 0){
        return INT_TYPE;
    } else {
        return NO_TYPE;
    }
}

/*
 * Used to create a let elem that has some value assigned to it (could be any type of node.)
 * Assign the id and make sure to free it.
*/
SYMBOL_TABLE_NODE *createLetElem(SYMBOL_TYPE sym_type, NUM_TYPE num_type, char *id, AST_NODE *s_expr, SYMBOL_TABLE_NODE *arglist)
{
    SYMBOL_TABLE_NODE *let_elem = createSymbolTableNode();

    let_elem->id = strdup(id);
    free(id);

    let_elem->symbolType = sym_type;
    let_elem->argTable = NULL;
    let_elem->type = num_type;
    let_elem->value = s_expr;
    let_elem->next = NULL;

    // ArgList set to NULL in all other cases
    if(arglist != NULL){
        let_elem->argTable = arglist;
    }

    return let_elem;
}

/*
 * Used when creating the symbol table list in a given scope. Before adding the new node,
 * we need to make sure that the node does not already exist in the scope (throw an error and free it if it does)
*/
SYMBOL_TABLE_NODE *createLetList(SYMBOL_TABLE_NODE *let_elem, SYMBOL_TABLE_NODE *let_list)
{
    SYMBOL_TABLE_NODE *table = let_elem;
    while (table != NULL)
    {
        if (strcmp(let_list->id, table->id) == 0)
        {
            if(table->symbolType != LAMBDA_TYPE && let_list->symbolType != LAMBDA_TYPE){
                warning("Duplicate assignment to symbol \"%s\" detected in the same scope! Only the first assignment is kept!", let_elem->id);
                freeSymbolTable(let_list);
                return let_elem;
            }
        }
        table = table->next;
    }

    let_list->next = let_elem;
    return let_list;
}

/*
 * Used to create an arguemnt symbol type. 
*/
SYMBOL_TABLE_NODE *createArg(char *id){
    SYMBOL_TABLE_NODE *arg = createSymbolTableNode();

    arg->type = ARG_TYPE;
    arg->next = NULL;
    arg->stack = NULL;
    arg->id = strdup(id);
    free(id);

    return arg;
}

/*
 * Link argument symbols together and return the list
*/
SYMBOL_TABLE_NODE *createArgList(char *id, SYMBOL_TABLE_NODE *argList){
    SYMBOL_TABLE_NODE *newArg = createArg(id);
    newArg->next = argList;

    return newArg;
}

/*
 * Creates a scope node. The created node is the parent of the child scope passed in.
 * Each AST_NODE in the let section must have its parent assigned to the child scope.
*/
AST_NODE *createScopeNode(SYMBOL_TABLE_NODE *let_section, AST_NODE *s_expr_right)
{
    AST_NODE *s_expr_left = createNode(SCOPE_NODE_TYPE);

    SYMBOL_TABLE_NODE *node = let_section;
    while (node != NULL)
    {
        if (s_expr_right != NULL && node->value != NULL)
        {
            node->value->parent = s_expr_right;
        }
        node = node->next;
    }

    s_expr_left->data.scope.child = s_expr_right;
    s_expr_right->parent = s_expr_left;
    s_expr_right->symbolTable = let_section;

    return s_expr_left;
}

/*
 * Used to create a symbol node from the production -> s_expr ::= SYMBOL
 * Also frees the id passed in from the parser.
*/
AST_NODE *createSymbolNode(char *id)
{
    AST_NODE *s_expr = createNode(SYM_NODE_TYPE);
    if ((s_expr->data.symbol.id = strdup(id)) == NULL)
    {
        yyerror("Strdup mem allocation failed!");
    }

    free(id);
    return s_expr;
}

/*
 * Base function used to handle the creation and memory allocation of a generic
 * symbol table node.
*/
SYMBOL_TABLE_NODE *createSymbolTableNode()
{
    SYMBOL_TABLE_NODE *node;

    size_t nodSize = sizeof(SYMBOL_TABLE_NODE);
    if ((node = malloc(nodSize)) == NULL)
    {
        yyerror("Memory allocation failure!");
        exit(1);
    }

    return node;
}

/*
 * Function to create a number node. Takes in the value (stored as double) and type
*/
AST_NODE *createNumberNode(double value, NUM_TYPE type)
{
    AST_NODE *node = createNode(NUM_NODE_TYPE);

    node->data.number.type = type;
    node->data.number.value = value;

    return node;
}

/*
 * Create a function node. 'Func' represents the function type, and the oplist is the operands.
 * Also have to set the parent of the operands to the returned AST_NODE to ensure proper
 * scope evaluation later on.
*/
AST_NODE *createFunctionNode(char *id, FUNC_TYPE func, AST_NODE *opList)
{
    AST_NODE *node = createNode(FUNC_NODE_TYPE);

    if(id != NULL){
        node->data.function.func_id = strdup(id);
        free(id);
    }

    AST_NODE *opList_head = opList;
    while (opList_head != NULL)
    {
        opList_head->parent = node;
        opList_head = opList_head->next;
    }

    node->data.function.func = func;
    node->data.function.opList = opList;

    return node;
}

AST_NODE *createConditionNode(AST_NODE *node1, AST_NODE *node2, AST_NODE *node3){
    AST_NODE *cond_node = createNode(COND_NODE_TYPE);

    // Set the operands (EVALUATE LATER)
    cond_node->data.cond.op1 = node1;
    cond_node->data.cond.op2 = node2;
    cond_node->data.cond.op3 = node3;

    // The parent of the operands needs to be set
    node1->parent = cond_node;
    node2->parent = cond_node;
    node3->parent = cond_node;

    return cond_node;
}

/*
 * Base function to create a node, takes in the type to assign the created node with.
*/
AST_NODE *createNode(AST_NODE_TYPE type)
{
    AST_NODE *node;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
        exit(1);
    }
    node->type = type;

    return node;
}

/*
 * Add a new AST_NODE to a list of existing nodes (used when creating a function's oplist)
*/
AST_NODE *addExpressionToList(AST_NODE *newExpr, AST_NODE *exprList)
{
    newExpr->next = exprList;
    exprList = newExpr;
    return exprList;
}

/*
 * Evaluate a condition node. If the first operand is > 0, return op2, else return op3
*/
RET_VAL evalCondNode(AST_NODE *node){
    AST_NUMBER condition = eval(node->data.cond.op1);

    if(condition.value != 0){
        return eval(node->data.cond.op2);
    }

    return eval(node->data.cond.op3);
}

/*
 * Function used to evaluate a func node. Call the appropriate handler based on the
 * func variable (as declared in the enum)
*/
RET_VAL evalFuncNode(AST_NODE *node)
{
    if (!node)
    {
        yyerror("NULL ast node passed into evalFuncNode!");
        return NAN_RET_VAL; // unreachable but kills a clang-tidy warning
    }

    FUNC_TYPE func_type = node->data.function.func;
    switch(func_type){
        case 0  ... 7:  return evalUnaryFuncs(node->data.function.opList, func_type);
        case 8  ... 14: return evalBinaryFuncs(node->data.function.opList, func_type);
        case 15 ... 19: return evalNaryFuncs(node->data.function.opList, func_type);
        case 20 ... 21: return evalNoArgFuncs(node->data.function.opList, func_type);
        default:        return evalCustomFunc(node);
    }
}

/*
 *  Scope nodes are simple. They just store a child and possibly a table.
*/
RET_VAL evalScopeNode(AST_NODE *s_expr)
{
    return eval(s_expr->data.scope.child);
}

/*
 *  Method used to evaluate a symbol node. First, we look through
 *  the curret scope's table, then go to the parent scope, all the while
 *  looping each table and comparing the id's.
*/
RET_VAL evalSymNode(AST_NODE *node)
{
    AST_NODE *scope = node;
    while (scope != NULL)
    {
        SYMBOL_TABLE_NODE *current_table = scope->symbolTable;
        SYMBOL_TABLE_NODE *temp = scope->symbolTable;
        while (current_table != NULL)
        {
            if (strcmp(current_table->id, node->data.symbol.id) == 0)
            {
                if(current_table->symbolType != LAMBDA_TYPE){
                    RET_VAL val;
                    if (current_table->value->type == NUM_NODE_TYPE){
                        val = eval(current_table->value);
                    } else{
                        AST_NODE *num_node = createNode(NUM_NODE_TYPE);
                        val = eval(current_table->value);
                        num_node->data.number = val;
                        freeNode(current_table->value);
                        current_table->value = num_node;
                    }

                    return evalTypeCasting(val, current_table->type);
                }
            }
            current_table = current_table->next;
        }
        // Check the arg_table last
        if(temp != NULL){
            SYMBOL_TABLE_NODE *curr_arg_list = temp->argTable;
            while(curr_arg_list != NULL){
                if(strcmp(curr_arg_list->id, node->data.symbol.id) == 0){
                    return curr_arg_list->stack->value;
                }
                curr_arg_list = curr_arg_list->next;
            }
        }
        scope = scope->parent;
    }

    warning("Undefined Symbol \"%s\" evaluated! NAN returned!", node->data.symbol.id);
    return NAN_RET_VAL;
}

/*
 * Used when evaluating a custom function. This will return the corresponding lambda function with
 * the id passed in.
*/
SYMBOL_TABLE_NODE *findLambdaFunction(AST_NODE *node){
    AST_NODE *scope = node;
    SYMBOL_TABLE_NODE *custom_func;
    while (scope != NULL){
        custom_func = scope->symbolTable;
        while (custom_func != NULL){
            if (strcmp(custom_func->id, node->data.function.func_id) == 0 && custom_func->symbolType == LAMBDA_TYPE){
                return custom_func;
            }
            custom_func = custom_func->next;
        }
        scope = scope->parent;
    }

    warning("Undefined function \"%s\" evaluated! NAN returned!", node->data.function.func_id);
    return NULL;
}

/*
 * Number nodes just store a AST_NUMBER, so return it.
*/
RET_VAL evalNumNode(AST_NODE *node)
{
    if (!node)
    {
        yyerror("NULL ast node passed into evalNumNode!");
        return NAN_RET_VAL;
    }

    if (node->type != NUM_NODE_TYPE)
    {
        yyerror("NON NUM_TYPE_NODE passed in to evalNumNode!");
        return NAN_RET_VAL;
    }

    return node->data.number;
}

/*
 * Evaluate the node based on the type.
*/
RET_VAL eval(AST_NODE *node)
{
    if (!node)
    {
        yyerror("NULL ast node passed into eval!");
        return NAN_RET_VAL;
    }

    switch (node->type)
    {
    case NUM_NODE_TYPE:
        return evalNumNode(node);
    case FUNC_NODE_TYPE:
        return evalFuncNode(node);
    case SYM_NODE_TYPE:
        return evalSymNode(node);
    case SCOPE_NODE_TYPE:
        return evalScopeNode(node);
    case COND_NODE_TYPE:
        return evalCondNode(node);
    default:
        yyerror("Unknown node type in eval!");
        return NAN_RET_VAL;
    }
}

/*
 * Evaluate the type casting on a given node, change the return value type when needed.
*/
RET_VAL evalTypeCasting(RET_VAL value, NUM_TYPE cast_type){
    if(cast_type == INT_TYPE && value.type == DOUBLE_TYPE){
        warning("Precision loss on int cast from %0.2lf to %d", value.value, (int)value.value);
        value.value = floor(value.value);
    }
    value.type = (cast_type < 2) ? cast_type : value.type;

    return value;
}

/*
 * Push argument values onto the arg stack used when evaluating functions.
 * First: evaluate the operands, then push them onto the stack (needed for nested
 * function calls to work right.)
*/
void evalFunctionPush(AST_NODE *opIterator, SYMBOL_TABLE_NODE *argIterator){
    STACK_NODE *head= NULL, *tail = NULL;
    while(opIterator != NULL){
        STACK_NODE *stack = malloc(sizeof(STACK_NODE));
        stack->value = eval(opIterator);
        if(head == NULL){
            stack->next = head;
            head = stack;
            tail = head;
        } else {
            tail->next = stack;
        }
        opIterator = opIterator->next;
    }

    while(argIterator != NULL){
        STACK_NODE *temp = malloc(sizeof(STACK_NODE));
        temp->value = head->value;
        temp->next = NULL;
        if(argIterator->stack == NULL){
            argIterator->stack = temp;
        } else {
            temp->next = argIterator->stack;
            argIterator->stack = temp;
        }
        argIterator = argIterator->next;
        head = head->next;
    }
}

/*
 * Pop argument values off of their respective arg types after calling a function 
*/
void evalFunctionPop(SYMBOL_TABLE_NODE *argIterator){
    while(argIterator != NULL){
        STACK_NODE *temp = argIterator->stack;
        argIterator->stack = argIterator->stack->next;
        free(temp);
        argIterator = argIterator->next;
    }
}

/*
 * Prints the type and value of a RET_VAL
*/ 
void printRetVal(RET_VAL val)
{
    switch (val.type)
    {
    case INT_TYPE:
        printf("Integer : %.lf\n", val.value);
        break;
    case DOUBLE_TYPE:
        printf("Double : %lf\n", val.value);
        break;
    default:
        printf("No Type : %lf\n", val.value);
        break;
    }
}

/*
 * Free the linked list created for the n-ary functions.
*/
void freeAstNumList(AST_NUM_LIST *head)
{
    while (head != NULL)
    {
        AST_NUM_LIST *temp = head;
        head = head->next;
        free(temp);
    }
}

/*
 *  Recursive function to free the nodes in the list. Check for any specific allocations first.
*/
void freeNode(AST_NODE *node)
{
    if (node == NULL)
    {
        return;
    }

    switch (node->type)
    {
        case FUNC_NODE_TYPE:  freeNode(node->data.function.opList); break;
        case SYM_NODE_TYPE:   free(node->data.symbol.id);           break;
        case SCOPE_NODE_TYPE: freeNode(node->data.scope.child);     break;
        case COND_NODE_TYPE:
            freeNode(node->data.cond.op1);
            freeNode(node->data.cond.op2);
            freeNode(node->data.cond.op3);
            break;
    }

    if (node->symbolTable != NULL)
    {
        freeSymbolTable(node->symbolTable);
    }

    freeNode(node->next);
    free(node);
}

/*
 *  Free any memory that was allocated (check if NULL for safety)
*/
void freeSymbolTable(SYMBOL_TABLE_NODE *node)
{
    if (node == NULL)
    {
        return;
    }

    if (node->id != NULL)
    {
        free(node->id);
    }

    if (node->value != NULL)
    {
        freeNode(node->value);
    }

    if (node->next != NULL)
    {
        freeSymbolTable(node->next);
    }

    free(node);
}
