#include "backend.h"
#include "frontend.h"

//---<Parser>-------------------------------------------

const char* string = NULL;


double GetG () 
{
    string = GetInputLine();

    SkipSpaces();

    double val = GetE();

    if (*string == '\0')
    {
        printf ("G: Got termination symbol, ending\n");
        return val;
    }
    else
    {
        printf ("Compilation error at char %c\n", *string);
    }


    return val;
}


double  GetE ()
{
    SkipSpaces();

    double val = GetT ();

    while (*string == '+' || *string == '-')
    {
        int last_op = *string;
        string++;
        double  cur_val = GetT ();

        if (last_op == '+')
            val += cur_val;
        else
            val -= cur_val;
    }   

    return val;
}


double  GetT()
{
    SkipSpaces();

    double val = GetP ();

    while (*string == '*' || *string == '/')
    {
        int last_op = *string;
        string++;
        double  cur_val = GetP ();

        if (last_op == '*')
            val *= cur_val;
        else
            val /= cur_val;
    }   

    printf ("T: Passing up %c\n", *string);

    return val;
}


double  GetP ()
{
    SkipSpaces();

    double val = 0;
    Operations operation = UNKNOWN;

    if (isalpha (*string))
    {
        if (*string == 'X')
        {
            string++;
            return 0;
        }
        int n = 0;
        char op_name[100] = "";

        sscanf (string, "%[^( ]%n", op_name, &n);
        printf ("Got argument %s and %d\n", op_name, n);

        operation = GetOpType (op_name);

        string += n;
    }
    
    if (*string == '(')
    {
        string++;

        val = GetE ();
        if (*string != ')') printf ("Missing \')\' - end of subexpression\n");
        
        string++;

        SkipSpaces ();
    }
    else
    {
        val = GetN ();
    }

    if (operation == UNKNOWN) return val;

    return CalcOneOp (NULL, val, operation);
}


double  GetN ()
{
    SkipSpaces();

    int val = 0;
    
    const char* OldString = string;

    while ('0' <= *string && *string <= '9')
    {
        val = val*10 + (*string - '0');
        string++;

        SkipSpaces();
    }

    printf ("N: Passing up %c\n", *string);

    return val;
}


char* GetInputLine ()
{
    char* buffer = (char*) calloc (MAX_SRC_LEN, sizeof (char));

    FILE* input_file = get_file ("data/input.txt", "r");

    fgets (buffer, MAX_SRC_LEN, input_file);

    fclose (input_file);

    return buffer;   
}


void SkipSpaces ()
{
    while (isspace (*string)) string++;
}


//---<Parser>-------------------------------------------


//--<Math>----------------------------------------------


TreeNode* GetDerivative (TreeNode* cur_node, TreeNode* root)
{
    if      (cur_node->type == NUM_T) return CreateNode (NUM_T, 0, UNKNOWN, nullptr, nullptr, nullptr);
    else if (cur_node->type == VAR_T) return CreateNode (NUM_T, 1, UNKNOWN, nullptr, nullptr, nullptr);
    else
    {
        switch (cur_node->value.op_val)
        {
        case ADD:
            $PRINT_N_RETURN(ADD (DL, DR));
        
        case SUB:
            $PRINT_N_RETURN(SUB (DL, DR));

        case MUL:
            $PRINT_N_RETURN(ADD(MUL(DL, CR), MUL(CL, DR)));
        
        case DIV:
            $PRINT_N_RETURN (DIV(SUB(MUL(DL, CR), MUL(CL, DR)), MUL(CR, CR)));

        case SIN:
            $PRINT_N_RETURN(MUL(COS(nullptr, CR), DR));

        case COS:
            $PRINT_N_RETURN(MUL(MUL(GET_DIGIT (-1), SIN(nullptr, CR)), DR));

        case LN:
            $PRINT_N_RETURN(MUL(DIV(GET_DIGIT (1), CR), DR));

        case TG: 
            $PRINT_N_RETURN(MUL(DIV(GET_DIGIT(1), COS(nullptr, CR)), DR));

        case CTG: 
            $PRINT_N_RETURN(MUL(MUL(DIV(GET_DIGIT(1), SIN(nullptr, CR)), DR), GET_DIGIT(-1)));

        
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
    int simpl_amount = 0;

    simpl_amount = 0;

    if (cur_node->left)  simpl_amount  += SimplifyTree (cur_node->left);
    if (cur_node->right) simpl_amount  += SimplifyTree (cur_node->right);
    
    if (!cur_node->left || !cur_node->right) return 0;

    if (cur_node->type == OP_T && cur_node->left->type != OP_T && cur_node->right->type != OP_T)
    {
        if (cur_node->value.op_val == MUL)
        {
            // printf ("Kek: ");
            // printf ("Now working with %lg and %lg\n", cur_node->left->value.dbl_val, cur_node->right->value.dbl_val);

            if      (isZero (cur_node->left->value.dbl_val) && cur_node->left->type == NUM_T)
            {
                printf ("Simplifying <Zero> case left\n");

                TransformNode (cur_node, NUM_T, 0, nullptr);
                return 1;
            }

            else if (isZero (cur_node->right->value.dbl_val) && cur_node->right->type == NUM_T)
            {
                printf ("Simplifying <Zero> case right\n");

                TransformNode (cur_node, NUM_T, 0, nullptr);
                return 1;
            }

            else if (isEqual (cur_node->right->value.dbl_val, 1))
            {
                printf ("Simplifying <one> case right\n");

                if (cur_node->left->type == NUM_T)
                {
                    TransformNode (cur_node, NUM_T, cur_node->left->value.dbl_val, nullptr);
                    return 1;
                }
                if (cur_node->left->type == VAR_T)
                {    
                    TransformNode (cur_node, VAR_T, 0, cur_node->left->value.var_name);
                    return 1;
                }
            }

            else if (isEqual (cur_node->left->value.dbl_val, 1))
            {
                printf ("Simplifying <one> case left\n");
                if (cur_node->right->type == NUM_T)
                {
                    TransformNode (cur_node, NUM_T, cur_node->right->value.dbl_val, nullptr);
                    return 1;
                }
                if (cur_node->right->type == VAR_T)
                {   
                    TransformNode (cur_node, VAR_T, 0, cur_node->right->value.var_name);
                    return 1;
                }
            }
        }
        if (cur_node->left->type == NUM_T && cur_node->right->type == NUM_T)
        {
            if (cur_node->value.op_val == ADD)
            {
                printf ("Oh, two numbers, ADD!\n");
                TransformNode (cur_node, NUM_T, cur_node->left->value.dbl_val + cur_node->right->value.dbl_val, nullptr);
                return 1;
            }
            else if (cur_node->value.op_val == MUL)
            {
                printf ("Oh, two numbers, MUL!\n");
                TransformNode (cur_node, NUM_T, cur_node->left->value.dbl_val * cur_node->right->value.dbl_val, nullptr);
                return 1;
            }
        }
    }

    return simpl_amount;
}


double CalcTree (TreeNode* node, double val)
{
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
    if (abs(num) <= 0.0000001) return true;

    return false;
}


bool isEqual (double num1, double num2)
{
    return (abs (num1 - num2) < 0.00000001) ? 1 : 0;
}






//--Math----------------------------------------------


TreeNode* GetTreeRoot ()
{
    FILE* tree_data = get_file ("data/input.txt", "r");
    
    TreeNode* root = BuildTree (tree_data);

    // DumpTree (root);
    
    fclose (tree_data);

    return root;
}


TreeNode* CreateNode (Types type, double dbl_val, Operations op_val, char* var_name,
                      TreeNode* left_child, TreeNode* right_child)
{
    printf ("Creating node with type %d\n", type);

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




//--Parser---------------------------------------------------

TreeNode* BuildTree (FILE* tree_info)
{
    char* buffer = GetTextBuffer (tree_info);
    int size     = (int) strlen(buffer);

    TreeNode* root = CreateNode(INIT_PARAMS);

    TreeNode* currnode = root;

    for (int counter = 0; counter < size; counter++)
    {
        while(isspace (*(buffer + counter)))
            counter += 1;

        if (*(buffer + counter) == ')')
        {
            if (currnode != root) currnode = currnode->parent;
            continue;
        }
        else if (*(buffer + counter) == '(')
        {
            if (currnode->left)
            {
                AddRightChild (currnode);
                currnode = currnode->right;
                continue;
            }
            else
            {
                AddLeftChild (currnode);
                currnode = currnode->left;
                continue;
            }
        }
        else
        {
            counter += FillCurrNode(currnode, buffer + counter);
        }
    }

    root = root->left;
    free(root->parent);
    root->parent = NULL;

    return root;
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
    else if (CMP (sqr)) return SQR;
    else if (CMP (ln)) return LN;
    

    else return UNKNOWN;
}

#undef CMP

