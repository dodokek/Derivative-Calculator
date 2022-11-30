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


TreeNode* CreateNode (Types type, double dbl_val, Operations op_val, char* var_name,
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
    char* string = GetInputLine();

    SkipSpaces (&string);

    TreeNode* root = GetExpression(&string); 

    if (*string == '\0')
        printf ("G: Got termination symbol, ending\n");
    else
        printf ("Compilation error at char %c\n", *string);

    return root;
}


TreeNode* GetExpression (char** string)
{
    SkipSpaces (string);

    TreeNode* top_operation_node = GetT (string);

    while (**string == '+' || **string == '-')
    {
        int last_op = **string;
        (*string)++;
        TreeNode* right_node = GetT (string);

        if (last_op == '+')
            top_operation_node = ADD (top_operation_node, right_node);
        else
            top_operation_node = SUB (top_operation_node, right_node);
    }   

    return top_operation_node;
}


TreeNode* GetT (char** string)
{
    SkipSpaces (string);

    TreeNode* top_operation_node = GetBracketExpower (string);

    while (**string == '*' || **string == '/')
    {
        int last_op = **string;
        (*string)++;

        TreeNode* right_node = GetBracketExpower (string);

        if (last_op == '*')
            top_operation_node = MUL (top_operation_node, right_node);
        else
            top_operation_node = DIV (top_operation_node, right_node);

    }       

    printf ("T: Passing up %c\n", **string);

    return top_operation_node;
}


TreeNode* GetBracketExpower (char** string)
{
    SkipSpaces (string);
    
    TreeNode* top_operation_node = GetBracketExp (string);

    while (**string == '^')
    {
        (*string)++;

        TreeNode* right_node = GetBracketExp (string);

        top_operation_node = POW (top_operation_node, right_node);
    }       

    printf ("K: Passing up %c\n", **string);

    return top_operation_node;
}


TreeNode* GetBracketExp (char** string)
{
    printf ("now working with char %c\n", **string);
    SkipSpaces (string);

    double val = 0;
    Operations operation = UNKNOWN;
    TreeNode* sub_node = nullptr;

    if (isalpha (**string))
    {
        if (**string == 'X')
        {
            (*string)++;
            return CreateNode (VAR_PARAMS(X));
        }

        int n = 0;
        char op_name[100] = "";

        sscanf (*string, "%[^( ]%n", op_name, &n);
        printf ("Got argument %s and %d\n", op_name, n);

        operation = GetOpType (op_name);

        *string += n;
    }
    
    if (**string == '(')
    {
        (*string)++;

        sub_node = GetExpression (string);

        if (**string != ')') printf ("Missing \')\' - end of subexpression\n");
        
        (*string)++;

        SkipSpaces (string);
    }
    else
    {
        sub_node = GetNumber (string);
    }

    if (operation == UNKNOWN) return sub_node;

    return GetOperationNode (sub_node, operation);
}


TreeNode*  GetNumber (char** string)
{
    SkipSpaces (string);

    int val = 0;
    
    while ('0' <= **string && **string <= '9')
    {
        val = val*10 + (**string - '0');
        (*string)++;

        SkipSpaces (string);
    }

    printf ("N: Passing up %c\n", **string);

    return CreateDigitNode (val);
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
        printf ("While getting operation nodes, error occured: %d\n", op);
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


void SkipSpaces (char** string)
{
    while (isspace (**string)) (*string)++;
}
    

void AddRightChild (TreeNode* cur_node)
{
    TreeNode* new_node = CreateNode(INIT_PARAMS);

    new_node->parent = cur_node;

    cur_node->right = new_node;
}


void AddLeftChild (TreeNode* cur_node)
{
    TreeNode* new_node = CreateNode(INIT_PARAMS);

    new_node->parent = cur_node;

    cur_node->left = new_node;
}


int FillCurrNode(TreeNode* currnode, char* buffer)
{
    double val = 0;
    char*  str = (char*) calloc(MAX_NAME_LEN, sizeof(char));
    int    len = 0;

    if (sscanf(buffer, "%lg%n", &val, &len) == 1)
    {
        currnode->type = NUM_T;
        currnode->value.dbl_val = val;

        return len - 1;
    }

    else if (sscanf(buffer, "%[^() ]%n", str, &len) == 1)
    {
        Operations op_type = UNKNOWN;

        if (op_type = GetOpType (str))
        {
            printf ("Got op type %d\n", op_type);
            currnode->type = OP_T;
            currnode->value.op_val = op_type;
        }
        else
        {
            currnode->type = VAR_T;
            currnode->value.var_name = str;
        }

        return len - 1;
    }

    return len;
}


ConfigParams GetConfigParams (char* file_name)
{
    FILE* conf_file = get_file (file_name, "r");

    char tmp_line[MAX_IMG_SRC_LEN] = "";
    fgets (tmp_line, MAX_SRC_LEN, conf_file);
    char str[MAX_IMG_SRC_LEN] = "";

    printf ("%s %d\n", tmp_line, atoi(tmp_line));
    printf ("Derivatives up to %d\n", atoi(tmp_line));

    fclose (conf_file);
}


//--Parser---------------------------------------------------


#define CMP(operation) strcmp (str, #operation) == 0

Operations GetOpType (const char str[])
{    
    if      (CMP (+)) return ADD;
    else if (CMP (-)) return SUB;
    else if (CMP (/)) return DIV;
    else if (CMP (*)) return MUL;
    else if (CMP (^)) return POW;
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

