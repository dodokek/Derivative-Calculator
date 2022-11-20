
#include "derivative.h"

int main()
{
    TreeNode* root = GetTreeRoot();

    TreeNode* d_root = GetDerivative (root);

    DrawTree (root);
    DrawTree (d_root);

    PrintInFile (d_root);

    DestructTree (root);
}


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


TreeNode* DestructTree (TreeNode* root)
{
    if (root->left)  DestructTree (root->left);
    if (root->right) DestructTree (root->right);

    root->value.var_name = nullptr;
    
    free (root);
    root = nullptr;

    return root;
}


//--Derivatives----------------------------------------------


TreeNode* GetDerivative (const TreeNode* cur_node)
{
    if      (cur_node->type == NUM_T) return CreateNode (NUM_T, 0, UNKNOWN, nullptr, nullptr, nullptr);
    else if (cur_node->type == VAR_T) return CreateNode (NUM_T, 1, UNKNOWN, nullptr, nullptr, nullptr);
    else
    {
        printf ("Hey hey, operation time!\n");
        switch (cur_node->value.op_val)
        {
        case ADD:
            return CreateNode (OP_T, 0, ADD, nullptr, GetDerivative (cur_node->left), GetDerivative (cur_node->right));
        
        case SUB:
            return CreateNode (OP_T, 0, SUB, nullptr, GetDerivative (cur_node->left), GetDerivative (cur_node->right));

        case MUL:
            return ADD(MUL(DL, CR), MUL(CL, DR));

        default:
            break;
        }
    }
}





//--Derivatives----------------------------------------------

//--Parser---------------------------------------------------

TreeNode* BuildTree (FILE* tree_info)
{
    char* buffer = GetTextBuffer (tree_info);
    int size     = (int)strlen(buffer);

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
    else if (CMP (sqr)) return SQR;

    else return UNKNOWN;
}

#undef CMP


//------------------------Dump----------------------------


void DumpTree (TreeNode* node)
{
    assert (node);

    printf ("Ptr[%p] : \n", node);
    
    if (node->type == NUM_T)
    {
        printf ("\t Node %s: left %p, right %p, parent %p, %d\n",
                node->value, node->left,
                node->right, node->parent, node->value);
    }
    else if (node->type == OP_T)
    {
        printf ("\t Node %s: left %p, right %p, parent %p, %d\n",
                node->value, node->left,
                node->right, node->parent, GetOpSign (node->value.op_val));
    }
    else
    {
        printf ("\t Node %s: left %p, right %p, parent %p, %d\n",
                node->value, node->left,
                node->right, node->parent, node->value);
    }

    if (node->left)  DumpTree (node->left);
    if (node->right) DumpTree (node->right);
}


void PrintInFile (TreeNode* root)
{
    FILE* out_file = get_file ("data/output.tex", "w+");

    const char header[] = R"(
    \documentclass{article}

    %  Русский язык

    \usepackage[T2A]{fontenc}			% кодировка
    \usepackage[utf8]{inputenc}			% кодировка исходного текста
    \usepackage[english,russian]{babel}	% локализация и переносы
    \usepackage{unicode-math}

    % Рисунки
    \usepackage{graphicx, float}
    \usepackage{wrapfig}


    \title{Домашнее задание 8}
    \author{Александр Морозов}
    \date{November 2022}

    \begin{document}
    )";

    fprintf (out_file, header);

    fprintf (out_file, "\\[");
    PrintInOrder (root, out_file);
    fprintf (out_file, "\\]");

    fprintf (out_file, "\n\n\t\\end{document}");

    fclose (out_file);

    // system ("xelatex -output-directory=data data/output.tex");
    // system ("del output.aux");
    // system ("del output.log");
    // system ("del output.out");
}


void PrintInOrder (TreeNode* node, FILE* out_file)
{
    fprintf (out_file, "{");
    printf ("Type %d Val %d\n", node->type, node->value.op_val);
    bool need_div = (node->type == OP_T && node->value.op_val == MUL);

    if (need_div) printf ("Loooooooool\n");

    if (need_div) fprintf (out_file, "(");
    if (node->left)  PrintInOrder (node->left,  out_file);
    if (need_div) fprintf (out_file, ")");

    if  (node->type == NUM_T)
        fprintf (out_file, "%lg", node->value.dbl_val);
    else if (node->type == OP_T)
        fprintf (out_file, "%s", GetOpSign (node->value.op_val));
    else
        fprintf (out_file, "%s", node->value.var_name);
    
    if (need_div) fprintf (out_file, "(");
    if (node->right) PrintInOrder (node->right, out_file);
    if (need_div) fprintf (out_file, ")");
    fprintf (out_file, "}");

} 


char* GetOpSign (Operations op)
{
    switch (op)
    {
    case ADD:
        return "+";

    case SUB:
        return "-";

    case DIV:
        return "/";

    case MUL:
        return "\\cdot";
    
    case SQR:
        return "№";

    case POW:
        return "^";

    default:
        return "?";
    }
} 


#define _print(...) fprintf (dot_file, __VA_ARGS__)

void DrawTree (TreeNode* root)
{
    static int img_counter = 0;

    FILE* dot_file = get_file ("data/graph.dot", "w+");
    
    // Writing header info
    const char header[] = R"(
    digraph g {
        dpi      = 200;
        fontname = "Comic Sans MS";
        fontsize = 20;
        rankdir   =  TB;
        edge [color = darkgrey, arrowhead = onormal, arrowsize = 1, penwidth = 1.2]
        graph[fillcolor = lightgreen, ranksep = 1.3, nodesep = 0.5,
        style = "rounded, filled",color = green, penwidth = 2]

    )";

    
    _print (header);

    InitGraphvisNode (root, dot_file);

    RecursDrawConnections (root, dot_file);

    _print ("}\n");
    
    // Executing dotfile and printing an image

    fclose (dot_file);

    char src[MAX_SRC_LEN] = "";

    sprintf (src, "dot -Tpng data/graph.dot -o data/pretty_tree%d.png", img_counter);

    system (src);

    img_counter++;

    return;
}


void InitGraphvisNode (TreeNode* node, FILE* dot_file)   // Recursivly initialises every node 
{
    if (node->type == NUM_T)
        _print ("Node%p[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"lightblue\","
                "label=\" {Type: number | value: %lg}\"] \n \n",
                node, node->value.dbl_val);

    else if (node->type == OP_T)
        _print ("Node%p[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"lightblue\","
                "label=\" {Type: operation | value: %s}\"] \n \n",
                node, GetOpSign(node->value.op_val));

    else if (node->type == VAR_T)
        _print ("Node%p[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"lightblue\","
                "label=\" {Type: variable | value: %s}\"] \n \n",
                node, node->value);

    else
    {
        _print ("Node%d[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"lightblue\","
                "label=\" {Op type: %d | value: unknown type}\"] \n \n",
                node, node->type);
    }

    if (node->left) InitGraphvisNode (node->left, dot_file);
    if (node->right) InitGraphvisNode (node->right, dot_file);

    return;
}


void RecursDrawConnections (TreeNode* node, FILE* dot_file)
{
    if (node->left)
    {
        _print ("Node%p->Node%p\n", node, node->left);
        RecursDrawConnections (node->left, dot_file);
    } 
    if (node->right)
    {
        _print ("Node%p->Node%p\n", node, node->right);
        RecursDrawConnections (node->right, dot_file);
    } 

    return;
}


#undef _print

//------------------------Dump----------------------------



