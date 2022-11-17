
#include "derivative.h"

int main()
{

    node* root = GetTreeRoot();

    DrawTree (root);

    DestructTree (root);
}


node* GetTreeRoot ()
{
    FILE* tree_data = get_file ("data/tree.txt", "r");
    
    node* root = BuildTree (tree_data);

    DumpTree (root);
    
    fclose (tree_data);

    return root;
}


node* InitTreeRoot (char name[])
{
    node* root = CreateNewNode();

    root->name = name;

    return root;
}


node* CreateNewNode ()
{
    node* new_node = (node*) calloc (1, sizeof (node));
    if (!new_node) return nullptr;

    new_node->left   = nullptr;
    new_node->right  = nullptr;
    new_node->parent = nullptr;

    new_node->name = (char*) calloc (MAX_NAME_LEN, sizeof (char)); 

    return new_node;
}


node* DestructTree (node* root)
{
    if (root->left)  DestructTree (root->left);
    if (root->right) DestructTree (root->right);

    root->name = nullptr;
    
    free (root);
    root = nullptr;

    return root;
}


// Positions 
node* InsertNode (char name[], node* parent, Positions position)
{
    node* new_node = CreateNewNode();
    
    new_node->name = name;
    new_node->parent = parent;

    if (position == LEFT)
    {
        if (parent->left == nullptr) parent->left = new_node;
        else
        {
            new_node->left = parent->left;
            parent->left->parent = new_node;
            parent->left = new_node;
        }
    }
    else if (position == RIGHT)
    {
        if (parent->right == nullptr) parent->right = new_node;
        else
        {
            new_node->left = parent->right;
            parent->right->parent = new_node;
            parent->right = new_node;
        }
    }

    return new_node;
}


node* FindNode (node* cur_node, const char name[])
{
    if (!cur_node) return nullptr;

    if (strcmp (cur_node->name, name) == 0) return cur_node;

    node* find_left = FindNode (cur_node->left, name);
    node* find_right = FindNode (cur_node->right, name);

    if (find_left) return find_left;
    if (find_right) return find_right;

    return nullptr;
}


node* BuildTree (FILE* tree_info)
{
    char* buffer = GetTextBuffer (tree_info);
    int size = strlen(buffer);

    node* root = CreateNewNode();

    node* currnode = root;

    for (int counter = 0; counter < size; counter++)
    {
        while(isspace(*(buffer + counter)))
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
                AddRightChild(currnode);
                currnode = currnode->right;
                continue;
            }
            else
            {
                AddLeftChild(currnode);
                currnode = currnode->left;
                continue;
            }
        }
        else
        {
            counter += FillCurrNode(currnode, buffer + counter);
        }
    }

    // difftree.anchor = difftree.anchor->leftchild;
    // free(difftree.anchor->ancestor);
    // difftree.anchor->ancestor = NULL;
    // difftree.size--;

    return root;
}
    

void AddRightChild (node* cur_node)
{
    node* new_node = CreateNewNode();

    new_node->parent = cur_node;

    cur_node->right = new_node;
}


void AddLeftChild (node* cur_node)
{
    node* new_node = CreateNewNode();

    new_node->parent = cur_node;

    cur_node->left = new_node;
}


int FillCurrNode(node* currnode, char* buffer)
{
    double val = 0;
    char* str = (char*) calloc(MAX_NAME_LEN, sizeof(char));
    int len = 0;

    // if (sscanf(input, "%lg%n", &val, &len) == 1)
    // {
    //     currnode->type = NUM_TYPE;
    //     currnode->val = val;

    //     return len - 1;
    // }

    if (sscanf(buffer, "%[^() ]%n", str, &len) == 1)
    {
        // if (OperType optype = IsOper(str))
        // {
        //     currnode->type = OP_TYPE;
        //     currnode->optype = optype;
        // }
        // else
        // {
        //     currnode->type = VAR_TYPE;
        //     currnode->varvalue = str;
        // }

        currnode->name = str;

        return len - 1;
    }

    return len;
}


//------------------------Tree builder--------------------


char* GetInput (char* buffer)
{
    fflush (stdin);

    gets (buffer);

    if (strlen (buffer) == 0)
    {    
        gets (buffer);
    }
    
    return buffer;
}


//------------------------Guessing mode. End-----------------------


//------------------------Object find mode----------------

Stack BuildAncestorsStack (node* cur_node)
{
    Stack ancestors = {0};
    StackCtor (&ancestors, 10);
    StackPush (&ancestors, cur_node);
    
    AddAncestor (cur_node->parent, &ancestors);
    
    return ancestors;
}


void AddAncestor (node* cur_node, Stack* ancestors)
{
    StackPush (ancestors, cur_node);

    if (cur_node->parent) AddAncestor (cur_node->parent, ancestors);
}

//------------------------Dump----------------------------

void DumpTree (node* node)
{
    assert (node);

    printf ("Ptr[%p] : \n", node);
    printf ("\t Node %s: left %p, right %p, parent %p, %s\n",
            node->name, node->left,
            node->right, node->parent, node->name);

    if (node->left)  DumpTree (node->left);
    if (node->right) DumpTree (node->right);
}


void PrintPreOrder (node* node, FILE* tree_data)
{
    fprintf (tree_data, "{\n%s\n", node->name);
    if (node->left)  PrintPreOrder (node->left,  tree_data);
    if (node->right) PrintPreOrder (node->right, tree_data);
    fprintf (tree_data, "}\n");
}


void PrintInOrder (node* node, FILE* tree_data)
{
    fprintf (tree_data, "{ ");
    if (node->left)  PrintPreOrder (node->left,  tree_data);

    fprintf (tree_data, "%s ", node->name);
    
    if (node->right) PrintPreOrder (node->right, tree_data);
    fprintf (tree_data, "} ");
} 


void PrintPostOrder (node* node, FILE* tree_data)
{
    if (node->left)  PrintPreOrder (node->left,  tree_data);
    if (node->right) PrintPreOrder (node->right, tree_data);
    fprintf (tree_data, "{\n%s\n", node->name);
    fprintf (tree_data, "}\n");
} 


#define _print(...) fprintf (dot_file, __VA_ARGS__)

void DrawTree (node* root)
{
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

    system ("dot -Tpng data/graph.dot -o data/pretty_tree.png");

    return;
}


void InitGraphvisNode (node* node, FILE* dot_file)   // Recursivly initialises every node 
{
    _print ("Node%p[shape=rectangle, color=\"red\", width=0.2, style=\"filled\","
            "fillcolor=\"lightblue\", label=\"%s\"] \n \n",
            node, node->name);
    
    if (node->left) InitGraphvisNode (node->left, dot_file);
    if (node->right) InitGraphvisNode (node->right, dot_file);

    return;
}


void RecursDrawConnections (node* node, FILE* dot_file)
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



