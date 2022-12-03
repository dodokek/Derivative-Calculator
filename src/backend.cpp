#include "backend.h"
#include "frontend.h"


//--<Math>----------------------------------------------


TreeNode* GetDerivative (TreeNode* cur_node, bool print_in_pdf)
{
    assert (cur_node != nullptr);

    if      (CUR_T == NUM_T) return CreateNode (NUM_T, 0, UNKNOWN, nullptr, nullptr, nullptr);
    else if (CUR_T == VAR_T) return CreateNode (NUM_T, 1, UNKNOWN, nullptr, nullptr, nullptr);
    else
    {
        switch (CUR_OP)
        {
        case ADD:
            $PRINT_PLUS_RETURN(ADD (DL, DR));
        
        case SUB:
            $PRINT_PLUS_RETURN(SUB (DL, DR));

        case MUL:
            $PRINT_PLUS_RETURN(ADD(MUL(DL, CR), MUL(CL, DR)));
        
        case DIV:
            $PRINT_PLUS_RETURN (DIV(SUB(MUL(DL, CR), MUL(CL, DR)), MUL(CR, CR)));

        case SIN:
            $PRINT_PLUS_RETURN(MUL(COS(nullptr, CR), DR));

        case COS:
            $PRINT_PLUS_RETURN(MUL(MUL(GET_DIGIT (-1), SIN(nullptr, CR)), DR));

        case LN:
            $PRINT_PLUS_RETURN(MUL(DIV(GET_DIGIT (1), CR), DR));

        case TG: 
            $PRINT_PLUS_RETURN(MUL(DIV(GET_DIGIT(1), POW(COS(nullptr, CR), GET_DIGIT(2))), DR));

        case CTG: 
            $PRINT_PLUS_RETURN(MUL(MUL(DIV(GET_DIGIT(1), POW(SIN(nullptr, CR), GET_DIGIT(2))), DR), GET_DIGIT(-1)));
        
        case POW:
            if (R_TYPE == NUM_T) $PRINT_PLUS_RETURN (MUL(MUL(GET_DIGIT(R_DBL), POW (CL, GET_DIGIT (R_DBL - 1))), DL));
        
        case UNKNOWN:
            printf ("Unknown operation\n");
            return nullptr;
            break;
            
        default:
            printf ("No such case for operation\n");
            return nullptr;
            break;
        }
    }
}


int SimplifyTree (TreeNode* cur_node)
{
    assert (cur_node != nullptr);

    int simpl_amount = 0;

    if (cur_node->left)  simpl_amount  += SimplifyTree (cur_node->left);
    if (cur_node->right) simpl_amount  += SimplifyTree (cur_node->right);
    
    if (!cur_node->left || !cur_node->right) return 0;

    if (CUR_T == OP_T && L_TYPE != OP_T && R_TYPE != OP_T)
    {
        if (CUR_OP == MUL)
        {
            if      (isZero (L_DBL) && L_TYPE == NUM_T)
            {
                printf ("Simplifying <Zero> case left\n");

                TransformNode (cur_node, NUM_T, 0, nullptr);
                return 1;
            }

            else if (isZero (R_DBL) && R_TYPE == NUM_T)
            {
                printf ("Simplifying <Zero> case right\n");

                TransformNode (cur_node, NUM_T, 0, nullptr);
                return 1;
            }

            else if (isEqual (R_DBL, 1))
            {
                printf ("Simplifying <one> case right\n");

                if (L_TYPE == NUM_T)
                {
                    TransformNode (cur_node, NUM_T, L_DBL, nullptr);
                    return 1;
                }
                if (L_TYPE == VAR_T)
                {    
                    TransformNode (cur_node, VAR_T, 0, L_VAR);
                    return 1;
                }
            }

            else if (isEqual (L_DBL, 1))
            {
                printf ("Simplifying <one> case left\n");
                if (R_TYPE == NUM_T)
                {
                    TransformNode (cur_node, NUM_T, R_DBL, nullptr);
                    return 1;
                }
                if (R_TYPE == VAR_T)
                {   
                    TransformNode (cur_node, VAR_T, 0, R_VAR);
                    return 1;
                }
            }
        }
        
        if (L_TYPE == NUM_T && R_TYPE == NUM_T)
        {
            if (CUR_OP == ADD)
            {
                printf ("Oh, two numbers, ADD!\n");
                TransformNode (cur_node, NUM_T, L_DBL + R_DBL, nullptr);
                return 1;
            }
            else if (CUR_OP == MUL)
            {
                printf ("Oh, two numbers, MUL!\n");
                TransformNode (cur_node, NUM_T, L_DBL * R_DBL, nullptr);
                return 1;
            }
        }
    }

    return simpl_amount;
}


double CalcTree (TreeNode* node, double val)
{
    assert (node != nullptr);

    double left_val = 0;
    double right_val = 0;
    if (node->type != OP_T) return 0;

    if (node->left)
    {
        if (node->left->type == OP_T) left_val = CalcTree (node->left, val);
        else
        {
            if (node->left->type == VAR_T) 
                left_val = val;
            else 
                left_val = node->left->value.dbl_val;
        }
    }
    if (node->right)
    {
        if (node->right->type == OP_T) right_val = CalcTree (node->right, val);
        else
        {
            if (node->right->type == VAR_T) 
                right_val = val;
            else 
                right_val = node->right->value.dbl_val;
        }
    }

    return CalcOneOp (left_val, right_val, node->value.op_val);
}


double CalcOneOp (double val1, double val2, Operations operation)
{
    switch (operation)
    {
    case ADD:
        return val1 + val2;

    case SUB:
        return val1 - val2;
    
    case DIV:
        if (isZero (val2)) return 0;
        return val1 / val2;

    case MUL:
        return val1 * val2;

    case POW:
        return pow(val1, val2);

    case SIN:
        return sin(val2);

    case COS:
        return cos(val2);

    case TG:
        return sin(val2)/cos(val2);

    case CTG:
        if (isZero (val2)) return 0;
        return cos(val2)/sin(val2);

    case LN:
        if (val2 <= 0) return 0;
        return log(val2);

    case LOG:
        return log(val2)/log(val1);

    default:
        printf ("Fucking shit! %d\n", operation);
        return 0;
        break;

    }
}


bool isZero (double num)
{
    printf ("recieved num %lg\n", num);
    if (abs(num) <= ACCURACY) return true;

    return false;
}


bool isEqual (double num1, double num2)
{
    return (abs (num1 - num2) < ACCURACY) ? 1 : 0;
}


//--Math----------------------------------------------


TreeNode* CreateNode (Types type, double dbl_val, Operations op_val, const char* var_name,
                      TreeNode* left_child, TreeNode* right_child)
{
    // printf ("Creating node with type %d\n", type);

    TreeNode* new_node = (TreeNode*) calloc (1, sizeof (TreeNode));
    if (!new_node) return nullptr;

    if      (type == NUM_T) new_node->value.dbl_val  = dbl_val;
    else if (type == VAR_T) new_node->value.var_name = var_name;
    else if (type == OP_T)  new_node->value.op_val   = op_val;
    
    new_node->left   = left_child;
    new_node->right  = right_child;
    new_node->parent = nullptr;
    new_node->type   = type;

    return new_node;
}


TreeNode* CopyNode (TreeNode* node_to_cpy)
{
    TreeNode* new_node = (TreeNode*) calloc (1, sizeof (TreeNode));
    if (!new_node) return nullptr;

    *new_node = *node_to_cpy;

    return new_node;
}


TreeNode* CreateDigitNode (double dbl_val)
{
    TreeNode* new_node = (TreeNode*) calloc (1, sizeof (TreeNode));
    if (!new_node) return nullptr;

    new_node->value.dbl_val  = dbl_val;
    
    new_node->left   = nullptr;
    new_node->right  = nullptr;
    new_node->parent = nullptr;
    new_node->type   = NUM_T;

    return new_node;
}


TreeNode* TransformNode (TreeNode* node, Types type, double dbl_val, const char* var_name)
{
    node->type = type;
    
    if (type == NUM_T)
        node->value.dbl_val = dbl_val;
    else
        node->value.var_name = var_name;

    if (node->left)  free (node->left);
    if (node->right) free (node->right);

    node->left = nullptr;
    node->right = nullptr;

}


TreeNode* DestructTree (TreeNode* root)
{
    if (root->left)  DestructTree (root->left);
    if (root->right) DestructTree (root->right);

    root->value.var_name = nullptr;
    
    free (root);
    root = nullptr;

    return root;
}


//---<Parser>-------------------------------------------


TreeNode* GetGrammar () 
{
    Token token_array[MAX_TOKENS];
    FillTokensArray (token_array);
    PrintTokens (token_array);

    int token_id = 0;
    int* cur_token_id = &token_id;

    TreeNode* root = GetExpression(token_array, cur_token_id); 

    if (CUR_TOKEN.value.op_val == TERMINATION_SYM)
        printf ("G: Got termination symbol, ending\n");
    else
        printf ("!!! Compilation error, cur Token: Type: %d, Dbl value: %lg. Line number %d. Op type: %d\n",
               CUR_TOKEN.type, CUR_TOKEN.value.dbl_val, CUR_TOKEN.line_number, CUR_TOKEN.value.op_val);

    return root;
}


TreeNode* GetExpression (Token token_array[], int* cur_token_id)
{
    TreeNode* top_operation_node = GetMlt (token_array, cur_token_id);

    while (CUR_TOKEN.value.op_val == ADD || CUR_TOKEN.value.op_val == SUB)
    {
        Operations last_op = CUR_TOKEN.value.op_val;
        *cur_token_id += 1;

        TreeNode* right_node = GetMlt (token_array, cur_token_id);

        if (last_op == ADD)
            top_operation_node = ADD (top_operation_node, right_node);
        else
            top_operation_node = SUB (top_operation_node, right_node);
    }   

    return top_operation_node;
}


TreeNode* GetMlt (Token token_array[], int* cur_token_id)
{
    TreeNode* top_operation_node = GetPower (token_array, cur_token_id);

    while (CUR_TOKEN.value.op_val == MUL || CUR_TOKEN.value.op_val == DIV)
    {
        Operations last_op = CUR_TOKEN.value.op_val;
        *cur_token_id += 1;

        TreeNode* right_node = GetPower (token_array, cur_token_id);

        if (last_op == MUL)
            top_operation_node = MUL (top_operation_node, right_node);
        else
            top_operation_node = DIV (top_operation_node, right_node);
    }       

    return top_operation_node;
}


TreeNode* GetPower (Token token_array[], int* cur_token_id)
{
    TreeNode* top_operation_node = GetBracketExp (token_array, cur_token_id);

    while (CUR_TOKEN.value.op_val == POW)
    {
        *cur_token_id += 1;

        TreeNode* right_node = GetBracketExp (token_array, cur_token_id);
        top_operation_node = POW (top_operation_node, right_node);
    }       

    return top_operation_node;
}


TreeNode* GetBracketExp (Token token_array[], int* cur_token_id)
{
    
    if (CUR_TOKEN.value.op_val == OPEN_BR)
    {
        *cur_token_id += 1;
        TreeNode* sub_node = GetExpression (token_array, cur_token_id);

        if (CUR_TOKEN.value.op_val == CLOSE_BR)
        {
            *cur_token_id += 1;
            return sub_node;
        }
        else 
        {
            printf ("Wrong brackets sequence\n");  
        }        
    }

    else if (CUR_TOKEN.type == OP_T)
    {
        Operations cur_op = CUR_TOKEN.value.op_val;

        *cur_token_id += 2; // Skipping operation + bracket
        TreeNode* sub_node = GetExpression (token_array, cur_token_id);

        if (CUR_TOKEN.value.op_val == CLOSE_BR)
        {
            *cur_token_id += 1;
            return GetOperationNode (sub_node, cur_op);
        }
        else 
        {
            printf ("Wrong brackets sequence\n");  
        }
    }
    else
    {
        return GetNumber (token_array, cur_token_id);
    }
}


TreeNode* GetNumber (Token token_array[], int* cur_token_id)
{
    if      (CUR_TOKEN.type == NUM_T)
    {
        *cur_token_id += 1;
        return CreateDigitNode (PREV_TOKEN.value.dbl_val);
    }

    else if (CUR_TOKEN.type == VAR_T)
    {
        *cur_token_id += 1;
        return CreateNode (VAR_T, 0, UNKNOWN, PREV_TOKEN.value.var_name, nullptr, nullptr);
    }
    else 
    {
        *cur_token_id += 1;
        printf ("Unexpected token, returning null\n");
        return nullptr;
    }
}


TreeNode* GetOperationNode (TreeNode* child_node, Operations op)
{
    switch (op)
    {
    case SIN:
        return SIN (nullptr, child_node);
    
    case COS:
        return COS (nullptr, child_node);

    case LN:
        return LN (nullptr, child_node);
    
    default:
        printf ("While GetMlting operation nodes, error occured: %d\n", op);
        break;
    }

    return 0;
}


char* GetInputLine ()
{
    char* buffer = (char*) calloc (MAX_SRC_LEN, sizeof (char));

    FILE* input_file = get_file ("data/input.txt", "r");

    fgets (buffer, MAX_SRC_LEN, input_file);

    fclose (input_file);

    return buffer;   
}


//-------Parser->Lecsical analysis----------------------------------

void FillTokensArray (Token* token_array)
{
    char* input = GetInputLine();
    int tokens_amount = 0;
    int input_len = strlen(input);

    for (int i = 0; i <= input_len;)
    {
        SkipSpaces (input, &i);

        if (isalpha(input[i]))
        {
            char* tmp_line = (input + i);
            while (isalpha (*tmp_line)) tmp_line++;

            if (*tmp_line == '(')
            {
                char op_name[100] = "";
                
                int len = 0;
                sscanf (input + i, "%[^( ]%n", op_name, &len);
                i += len;
                
                Operations operation = GetOpType (op_name);
                TOP_TOKEN = CreateToken (OP_T, 0, operation, i); 
                tokens_amount++;
            }   
            else // variable handler
            {
                TOP_TOKEN = CreateToken (VAR_T, 0, UNKNOWN, i);

                int len = 0;
                sscanf (input + i, "%[^+-*/() ]%n", TOP_TOKEN.value.var_name, &len);
                i += len;

                printf ("Working with var, its len: %d, String %s\n", len, input + i);
                tokens_amount++;
            }
        }
        else if (isdigit (input[i]))
        {
            printf ("Proccessing digit %c\n", input[i]);

            double num = 0;

            int len;
            sscanf (input + i, "%lg%n", &num, &len);
            i += len;

            TOP_TOKEN = CreateToken (NUM_T, num, UNKNOWN, i);
            tokens_amount++;

        }
        else
        {
            printf ("Proccessing operation %c\n", input[i]);

            Operations operation = UNKNOWN;
            operation = GetOpType (input + i);
            printf ("Input: %c, Op type: %d\n", input[i], operation);
        
            TOP_TOKEN = CreateToken (OP_PARAMS(operation));
            
            tokens_amount++;
            i++;
        }
    } 
}


void SkipSpaces (char* string, int* i)
{
    while (isspace (string[*i])) (*i)++;
}


Token CreateToken (Types type, double dbl_val, Operations op_t, int line_number)
{
    printf ("====Creating token with type %d and op val %d====\n", type, op_t);

    Token* new_token = (Token*) calloc (1, sizeof (Token));

    if      (type == NUM_T) new_token->value.dbl_val  = dbl_val;
    else if (type == VAR_T) new_token->value.var_name = (char*) calloc (MAX_TOKEN_LEN, sizeof (char));
    else if (type == OP_T)  new_token->value.op_val   = op_t;
    
    new_token->type = type;
    new_token->line_number = line_number;

    return *new_token;
}


void PrintTokens (Token* token_array)
{
    int i = 0;
    while (token_array[i].value.op_val != TERMINATION_SYM)
    {
        printf ("Token. Type: %d, Dbl value: %lg. Line number %d. Op type: %d\n",
               token_array[i].type, token_array[i].value.dbl_val, token_array[i].line_number, token_array[i].value.op_val);

        i++;
    }
}


//-------Parser->Lecsical analysis----------------------------------


//--Parser.End---------------------------------------------------


#define CMP(operation) strcmp (str, #operation) == 0

Operations GetOpType (char str[])
{    
    if      (str[0] == '+') return ADD;
    else if (str[0] == '-') return SUB;
    else if (str[0] == '*') return MUL;
    else if (str[0] == '/') return DIV;
    else if (str[0] == '^') return POW;
    else if (str[0] == '(') return OPEN_BR;
    else if (str[0] == ')') return CLOSE_BR;
    else if (str[0] == '\0') return TERMINATION_SYM;
    else if (CMP (sin)) return SIN;
    else if (CMP (cos)) return COS;
    else if (CMP (tg))  return TG;
    else if (CMP (ctg)) return CTG;
    else if (CMP (ln)) return LN;
    else if (CMP (arccos)) return ARCCOS;
    else if (CMP (arcsin)) return ARCSIN;


    else return UNKNOWN;
}

#undef CMP

