#include "cilisp.h"

// Array of all binary funcs
RET_VAL (*binaryFuncs[])(RET_VAL, RET_VAL) = {
    evalSubFunc, evalDivFunc, evalRemFunc, evalPowFunc,
    evalEqualFunc, evalLessFunc, evalGreaterFunc,
};

// Array of all unary funcs
RET_VAL (*unaryFuncs[])(AST_NUMBER) = {
    evalNegFunc, evalAbsFunc, evalExpFunc, evalExp2Func,
    evalLogFunc, evalSqrtFunc, evalCbrtFunc, evalPrintFunc,
};

// Array of all n-ary funcs
RET_VAL (*naryFuncs[])(AST_NUM_LIST*) = {
    evalMultFunc, evalAddFunc, evalHypotFunc, evalMaxFunc, evalMinFunc,
};

// Made global so I can access it in other functions
char *funcNames[] = {
    "neg", "abs", "exp", "exp2", "log", "sqrt", "cbrt", "print", "sub", "div", "remainder",
    "pow", "equal", "less", "greater", "mult", "add", "hypot", "max", "min", "rand", "read",
    // the empty string below must remain the last element
    ""
};

FUNC_TYPE resolveFunc(char *funcName)
{
    // Array of string values for function names.
    // Must be in sync with members of the FUNC_TYPE enum in order for resolveFunc to work.
    // For example, funcNames[NEG_FUNC] should be "neg"
    int i = 0;
    while (funcNames[i][0] != '\0')
    {
        if (strcmp(funcNames[i], funcName) == 0)
        {
            return i;
        }
        i++;
    }
    return CUSTOM_FUNC;
}

RET_VAL evalCustomFunc(AST_NODE *node){
    SYMBOL_TABLE_NODE *custom_func = findLambdaFunction(node);
    if(custom_func == NULL){
        return NAN_RET_VAL;
    }

    AST_NODE *opIterator = node->data.function.opList; 
    SYMBOL_TABLE_NODE *argIterator = custom_func->argTable;

    evalFunctionPush(opIterator, argIterator);
    RET_VAL returnVal = evalTypeCasting(eval(custom_func->value), custom_func->type);
    evalFunctionPop(custom_func->argTable);
    
    return returnVal;
}

// ------------------------ BEGIN UNARY  FUNCS  -----------------------------//
RET_VAL evalUnaryFuncs(AST_NODE *node, FUNC_TYPE func_type){
    RET_VAL operand, product;
    if(node == NULL){
        warning("%s called with no operands! nan returned!", funcNames[func_type]);
        return NAN_RET_VAL;
    } 
    if(node->next != NULL){
        warning("%s called with extra (ignored) operands!", funcNames[func_type]);
    }

    operand = eval(node);
    
    product = unaryFuncs[func_type](operand);

    return product;
}

RET_VAL evalNegFunc(AST_NUMBER op){
    op.value = op.value * -1;
    return op;
}
RET_VAL evalAbsFunc(AST_NUMBER op){
    op.value = (op.value < 0) ? op.value * -1: op.value; 
    return op;
}
RET_VAL evalExpFunc(AST_NUMBER op){
    op.type = DOUBLE_TYPE;
    op.value = exp(op.value);
    return op;
}
RET_VAL evalExp2Func(AST_NUMBER op){
    op.type = (op.value < 0) ? DOUBLE_TYPE:op.type;
    op.value = pow(2.0, op.value);
    return op;
}
RET_VAL evalLogFunc(AST_NUMBER op){
    op.type = DOUBLE_TYPE;
    op.value = log(op.value);
    return op;
}
RET_VAL evalSqrtFunc(AST_NUMBER op){
    op.type = DOUBLE_TYPE;
    op.value = sqrt(op.value);
    return op;
}
RET_VAL evalCbrtFunc(AST_NUMBER op){
    op.type = DOUBLE_TYPE;
    op.value = cbrt(op.value);
    return op;
}
RET_VAL evalPrintFunc(AST_NUMBER op){
    printRetVal(op);
    return op;
}
// ------------------------ BEGIN BINARY FUNCS ------------------------------//
RET_VAL evalBinaryFuncs(AST_NODE *node, FUNC_TYPE func_type){
    RET_VAL operand1, operand2, product;

    if(node == NULL){
        warning("%s called with no operands! nan returned!", funcNames[func_type]);
        return NAN_RET_VAL;
    } else if(node->next == NULL){
        warning("%s called with only one arg! nan returned!", funcNames[func_type]);
        return NAN_RET_VAL;
    }

    operand1 = eval(node);
    operand2 = eval(node->next);

    product = binaryFuncs[func_type - 8](operand1, operand2);

    if(node->next->next != NULL){
        warning("%s called with extra (ignored) operands!", funcNames[func_type]);
    }

    return product;
}

RET_VAL evalSubFunc(RET_VAL op1, RET_VAL op2){
    op1.value = op1.value - op2.value;
    op1.type = op1.type || op2.type;
    return op1;
}
RET_VAL evalDivFunc(RET_VAL op1, RET_VAL op2){
    op1.type = op1.type || op2.type;
    if(!op1.type){
        op1.value = (double)((int)op1.value / (int)op2.value);
        return op1;
    }
    op1.value = op1.value / op2.value;
    return op1;
}
RET_VAL evalRemFunc(RET_VAL op1, RET_VAL op2){
    op1.type = op1.type || op2.type;
    double q = floor(op1.value / op2.value);
    double r = fmod(op1.value, op2.value);

    if(r < 0.0){
        r += fabs(op2.value);
    }
    op1.value = r;

    return op1;
}
RET_VAL evalPowFunc(RET_VAL op1, RET_VAL op2){
    op1.value = pow(op1.value, op2.value);
    op1.type = op1.type || op2.type;
    return op1;
}
RET_VAL evalEqualFunc(RET_VAL op1, RET_VAL op2){
    RET_VAL condition;
    condition.type = INT_TYPE;
    if(op1.value == op2.value){
        condition.value = 1;
    } else {
        condition.value = 0;
    }
    return condition;
}
RET_VAL evalLessFunc(RET_VAL op1, RET_VAL op2){
    RET_VAL condition;
    condition.type = INT_TYPE;
    if(op1.value < op2.value){
        condition.value = 1;
    } else {
        condition.value = 0;
    }
    return condition;
}
RET_VAL evalGreaterFunc(RET_VAL op1, RET_VAL op2){
    RET_VAL condition;
    condition.type = INT_TYPE;
    if(op1.value > op2.value){
        condition.value = 1;
    } else {
        condition.value = 0;
    }
    return condition;
}

// ------------------------ BEGIN N-ARY FUNCS ------------------------------//
RET_VAL evalNaryFuncs(AST_NODE *node, FUNC_TYPE func_type){
    RET_VAL operand1, operand2, product;
    if(node == NULL){
        RET_VAL ret;
        switch (func_type)
        {
        case ADD_FUNC:
            ret.value = 0;
            ret.type = INT_TYPE;
            break;        default:
        case MULT_FUNC:
            ret.value = 1;
            ret.type = INT_TYPE;
            break;
        case HYPOT_FUNC:
            ret.value = 0;
            ret.type = DOUBLE_TYPE;
            break;
        case MAX_FUNC:
        case MIN_FUNC:
            ret = NAN_RET_VAL;
            warning("%s called with no operands! nan returned!", funcNames[func_type]);
            return ret;
        }
        warning("%s called with no operands! %d returned!", funcNames[func_type], (int)ret.value);
        return ret;
    } else if(node->next == NULL){
        if(node->type == NUM_NODE_TYPE){
            if(func_type == HYPOT_FUNC){
                node->data.number.type |= 1;
                return node->data.number;
            }
            return node->data.number;
        } else {
            return eval(node);
        }
    }

    AST_NUM_LIST *head = NULL;
    while(node != NULL){
        AST_NUM_LIST *num = malloc(sizeof(AST_NUM_LIST));
        num->number = eval(node);
        num->next = head;
        head = num;
        node = node->next;
    }

    product = naryFuncs[func_type - 15](head);
    freeAstNumList(head);

    return product;
}

RET_VAL evalAddFunc(AST_NUM_LIST *node){
    RET_VAL sum;
    sum.value = 0;
    sum.type = INT_TYPE;
    while(node != NULL){
        sum.value += node->number.value;
        sum.type |= node->number.type;
        node = node->next;
    }

    return sum;
}
RET_VAL evalMultFunc(AST_NUM_LIST *node){
    RET_VAL sum;
    sum.value = 1;
    sum.type = INT_TYPE;
    while(node != NULL){
        sum.value *= node->number.value;
        sum.type |= node->number.type;
        node = node->next;
    }

    return sum;
}
RET_VAL evalHypotFunc(AST_NUM_LIST *node){
    RET_VAL hypot = evalAddFunc(node);
    hypot.value = 0;
    hypot.type = DOUBLE_TYPE;
    while(node != NULL){
        hypot.value += pow(node->number.value, 2.0);
        node = node->next;
    }
    hypot.value = sqrt(hypot.value);
    return hypot;
}
RET_VAL evalMaxFunc(AST_NUM_LIST *node){
    RET_VAL max;
    max.value = -53.67;
    while(node != NULL){
        if(max.value == -53.67 || node->number.value > max.value){
            max.value = node->number.value;
            max.type = node->number.type;
        }
        node = node->next;
    }

    return max;
}
RET_VAL evalMinFunc(AST_NUM_LIST *node){
    RET_VAL min;
    min.value = -53.67;
    while(node != NULL){
        if(min.value == -53.67 || node->number.value < min.value){
            min.value = node->number.value;
            min.type = node->number.type;
        }
        node = node->next;
    }

    return min;
}

RET_VAL evalNoArgFuncs(AST_NODE *op_list, FUNC_TYPE func_type){
    if(op_list != NULL){
        warning("%s called with extra (ignored) operands!", funcNames[func_type]);
    }

    return (func_type == 20) ? evalRandFunc() : evalReadFunc();
}

RET_VAL evalRandFunc(){
    RET_VAL ret;
    ret.type = DOUBLE_TYPE;
    ret.value = (double)rand() / RAND_MAX;

    return ret;
}

/*
 * Easier way to do this? Maybe, but this was fun. Small FSM used to read in the entered number
 * character by character and detect what type of number it is, if it even is one.
*/
RET_VAL evalReadFunc(){
    printf("read :: ");

    char buffer[250];
    char string_value[100];

    if((fscanf(read_target, "%s", buffer)) != 1){
        warning("Invalid read entry! NAN returned!");
        return NAN_RET_VAL;
    }

    int i = 0, len = strlen(buffer);
    STATE state = START_STATE;
    while(i < len && state != INVALID_STATE){ 
        switch(state){
            case START_STATE:
                switch(buffer[i]){
                    case '0' ... '9':
                    case '-':
                    case '+':
                        state = INT_STATE;
                        break;
                    default:
                        state = INVALID_STATE;
                }
                break;
            case INT_STATE:
                switch(buffer[i]){
                    case '0' ... '9':
                        break;
                    case '.':
                        state = FLOAT_STATE;
                        break;
                    default:
                        state = INVALID_STATE;
                }
                break;
            case FLOAT_STATE:
                switch(buffer[i]){
                    case '0' ... '9':
                        break;
                    default:
                        state = INVALID_STATE;
                }
                break;
        }
        i++;
    }

    if(state == START_STATE || state == INVALID_STATE){
        warning("Invalid read entry! NAN returned!");
        return NAN_RET_VAL;
    }

    RET_VAL value;
    value.value = atof(buffer);
    value.type = (state == 1) ? INT_TYPE : DOUBLE_TYPE;

    return value;
}
