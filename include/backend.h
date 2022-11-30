#ifndef BACKEND_H
#define BACKEND_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "fileUtils.h"
#include "dsl.h"
#include "stringUtils.h"

//-----------------------------------------------


enum Sizes
{
    MAX_IMG_SRC_LEN = 100,
    STACK_INIT_CAP = 5,
    MAX_NAME_LEN = 100,
    MAX_SRC_LEN = 500,
    MAX_VARIABLES = 10,
};


enum Types
{
    NUM_T,
    VAR_T,
    OP_T,
};


enum OutputType
{
    NO_TEX_OUT,
    TEX_OUT,
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
    TG,
    CTG,
    ARCCOS,
    ARCSIN,
    LN,
    LOG,
};


const double ACCURACY = 0.000001;


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


struct Variable
{
    double value;
    char name[MAX_NAME_LEN];
};


struct ConfigParams
{
    int derivative_rang;
    int macloren_rang;
    Variable variables[MAX_VARIABLES];
};



//----------------------------------------------------------

char* GetInputLine ();

TreeNode* GetGrammar ();

TreeNode* GetNumber (char** string);

TreeNode* GetExpression (char** string);

TreeNode* GetT (char** string);

TreeNode* GetBracketExpower (char** string);

TreeNode* GetBracketExp (char** string);

void SkipSpaces (char** string);

ConfigParams GetConfigParams (char* file_name);

TreeNode* GetTreeRoot ();

TreeNode* CreateNode (Types type,           double dbl_val,
                      Operations op_val,    char* var_name,
                      TreeNode* left_child, TreeNode* right_child);

TreeNode* TransformNode (TreeNode* node, Types type, double dbl_val, const char* var_name);

TreeNode* CopyNode (TreeNode* node_to_cpy);

TreeNode* BuildTree (FILE* tree_info);

void AddRightChild (TreeNode* cur_node);

void AddLeftChild (TreeNode* cur_node);

int FillCurrNode(TreeNode* currnode, char* buffer);

TreeNode* DestructTree (TreeNode* root);

Operations GetOpType (const char str[]);

TreeNode* GetDerivative (TreeNode* cur_node, bool print_in_pdf);

bool IsFictiveZero (Operations op);

TreeNode* CreateDigitNode (double dbl_val);

int SimplifyTree (TreeNode* cur_node);

bool isZero (double num);

bool isEqual (double num1, double num2);

double CalcTree (TreeNode* node, double val);

double CalcOneOp (double val1, double val2, Operations operation);

TreeNode* GetOperationNode (TreeNode* child_node, Operations op);

#endif