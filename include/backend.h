#ifndef BACKEND_H
#define BACKEND_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "fileUtils.h"
#include "dsl.h"
#include "stringUtils.h"

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
    MAX_SRC_LEN = 500,
};


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


//---------------------------------------------


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


//----------------------------------------------------------


TreeNode* GetTreeRoot ();

TreeNode* CreateNode (Types type,           double dbl_val,
                      Operations op_val,    char* var_name,
                      TreeNode* left_child, TreeNode* right_child);

TreeNode* CopyNode (const TreeNode* node_to_cpy);

TreeNode* BuildTree (FILE* tree_info);

void AddRightChild (TreeNode* cur_node);

void AddLeftChild (TreeNode* cur_node);

int FillCurrNode(TreeNode* currnode, char* buffer);

TreeNode* DestructTree (TreeNode* root);

Operations GetOpType (const char str[]);

TreeNode* GetDerivative (const TreeNode* cur_node);

TreeNode* CreateDigitNode (double dbl_val);

int SimplifyTree (TreeNode* cur_node);

bool isZero (double num);

#endif