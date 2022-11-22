#include "backend.h"
#include "frontend.h"

//--<Math>----------------------------------------------


TreeNode* GetDerivative (const TreeNode* cur_node)
{
    if      (cur_node->type == NUM_T) return CreateNode (NUM_T, 0, UNKNOWN, nullptr, nullptr, nullptr);
    else if (cur_node->type == VAR_T) return CreateNode (NUM_T, 1, UNKNOWN, nullptr, nullptr, nullptr);
    else
    {
        switch (cur_node->value.op_val)
        {
        case ADD:
            return ADD (DL, DR);
        
        case SUB:
            return SUB (DL, DR);

        case MUL:
            return ADD(MUL(DL, CR), MUL(CL, DR));
        
        case SIN:
            return MUL(COS(nullptr, CR), DR);

        case COS:
            return MUL(MUL(GET_DIGIT (-1), SIN(nullptr, CR)), DR);

        case LN:
            return MUL(DIV(GET_DIGIT (1), CR), DR);

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
    
    if (cur_node->type == OP_T && cur_node->left->type != OP_T && cur_node->right->type != OP_T)
    {
        if (cur_node->value.op_val == MUL)
        {
            if      (isZero (cur_node->left->value.dbl_val))
            {
                TransformNode (cur_node, NUM_T, 0, nullptr);
                return 1;
            }

            else if (isZero (cur_node->right->value.dbl_val))
            {
                TransformNode (cur_node, NUM_T, 0, nullptr);
                return 1;
            }

            else if (isEqual (cur_node->right->value.dbl_val, 1))
            {
                if (cur_node->left->type == NUM_T)
                    TransformNode (cur_node, NUM_T, cur_node->left->value.dbl_val, nullptr);
                
                if (cur_node->left->type == VAR_T)
                    TransformNode (cur_node, VAR_T, 0, cur_node->left->value.var_name);

                return 1;
            }

            else if (isEqual(cur_node->left->value.dbl_val, 1))
            {
                if (cur_node->right->type == NUM_T)
                    TransformNode (cur_node, NUM_T, cur_node->right->value.dbl_val, nullptr);
                
                if (cur_node->right->type == VAR_T)
                    TransformNode (cur_node, VAR_T, 0, cur_node->right->value.var_name);

                return 1;
            }

            else if (cur_node->left->type == NUM_T && cur_node->right->type == NUM_T)
            {
                TransformNode (cur_node, NUM_T, cur_node->left->value.dbl_val + cur_node->right->value.dbl_val, nullptr);
                return 1;
            }
        }
    }

    return simpl_amount;
}



bool isZero (double num)
{
    if (num <= 0.0000001) return true;

    return false;
}


bool isEqual (double num1, double num2)
{
    return (abs (num1 - num2) < 0.00000001) ? 1 : 0;
}


//--Math----------------------------------------------


TreeNode* GetTreeRoot ()
{
    FILE* tree_data = get_file ("data/tree.txt", "r");
    
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


TreeNode* CopyNode (const TreeNode* node_to_cpy)
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
    else if (CMP (sqr)) return SQR;
    else if (CMP (ln)) return LN;

    else return UNKNOWN;
}

#undef CMP

