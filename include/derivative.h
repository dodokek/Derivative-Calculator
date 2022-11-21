#ifndef derivative_H
#define derivative

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdarg.h>

#include "fileUtils.h"
#include "stack.h"
#include "stringUtils.h"
#include "dsl.h"

//-----------------------------------------------

#define INIT_PARAMS OP_T, 0, UNKNOWN, nullptr, nullptr, nullptr


//-----------------------------------------------

enum Positions
{
    LEFT,
    RIGHT,
};


enum Sizes
{
    MAX_IMG_SRC_LEN = 100,
    STACK_INIT_CAP = 5,
    MAX_NAME_LEN = 100,
    MAX_SRC_LEN =500,
};


enum PlayMode
{
    EXIT = 0,
    GUESS = 1,
    LISTING = 2,
    COMPARISON = 3,
};


//-----------------------------------------------

enum Types
{
    NUM_T,
    VAR_T,
    OP_T,
};


enum Operations
{
    UNKNOWN,
    ADD,
    SUB,
    DIV,
    MUL,
    SQR,
    POW,
    SIN,
    COS,
    ARCCOS,
    ARCSIN,
    LN,
    LOG,
};


union Values
{
    double dbl_val;
    Operations op_val;
    const char* var_name;
};


struct TreeNode
{
    Types type; 
    
    TreeNode* parent;

    TreeNode* left;
    TreeNode* right;

    Values value;
};


bool isNegativeAns (TreeNode* cur_node);

TreeNode* GetNodeFromUser (TreeNode* root);

TreeNode* GetTreeRoot ();

TreeNode* InsertNode (char name[], TreeNode* parent, Positions position = LEFT);

TreeNode* CreateNode (Types type, double dbl_val, Operations op_val, char* var_name,
                      TreeNode* left_child, TreeNode* right_child);

TreeNode* CopyNode (const TreeNode* node_to_cpy);

TreeNode* InitTreeRoot (char name[]);

char* GetInput (char* buffer);

void AddNode (TreeNode* cur_node);

void DumpTree (TreeNode* root);

void DrawTree (TreeNode* root);

void PrintInOrder (TreeNode* node, FILE* out_file);

bool isNeedDivision (TreeNode* op_node);

char* GetOpSign (Operations op);

void PrintPostOrder (TreeNode* node, FILE* out_file);

void PrintObject (TreeNode* node_to_print);

Stack BuildAncestorsStack (TreeNode* cur_node);

void AddAncestor (TreeNode* cur_node, Stack* ancestors);

void InitGraphvisNode (TreeNode* node, FILE* dot_file);

void RecursDrawConnections (TreeNode* node, FILE* dot_file);

TreeNode* FindNode (TreeNode* cur_node, const char name[]);

TreeNode* BuildTree (FILE* tree_info);

void AddRightChild (TreeNode* cur_node);

void AddLeftChild (TreeNode* cur_node);

int FillCurrNode(TreeNode* currnode, char* buffer);

TreeNode* DestructTree (TreeNode* root);

Operations GetOpType (const char str[]);

void PrintInFile (TreeNode* root);

TreeNode* GetDerivative (const TreeNode* cur_node);

TreeNode* CreateDigitNode (double dbl_val);

#endif