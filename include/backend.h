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
    MAX_TOKEN_LEN = 10,
    MAX_TOKENS = 200,
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
    OPEN_BR,
    CLOSE_BR,
    TERMINATION_SYM,
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


struct Token
{
    Types type;
    Values value;

    int line_number;
};


//----------------------------------------------------------

char* GetInputLine ();

TreeNode* GetGrammar ();

TreeNode* GetExpression (Token token_array[], int* cur_token_id);

TreeNode* GetMlt (Token token_array[], int* cur_token_id);

TreeNode* GetPower (Token token_array[], int* cur_token_id);

TreeNode* GetNumber (Token token_array[], int* cur_token_id);

TreeNode* GetBracketExp (Token token_array[], int* cur_token_id);

void SkipSpaces (char* string, int* i);

ConfigParams GetConfigParams (char* file_name);

TreeNode* GetMltreeRoot ();

TreeNode* CreateNode (Types type,           double dbl_val,
                      Operations op_val,    const char* var_name,
                      TreeNode* left_child, TreeNode* right_child);

TreeNode* TransformNode (TreeNode* node, Types type, double dbl_val, const char* var_name);

TreeNode* CopyNode (TreeNode* node_to_cpy);

TreeNode* BuildTree (FILE* tree_info);

TreeNode* DestructTree (TreeNode* root);

Operations GetOpType (char str[]);

TreeNode* GetDerivative (TreeNode* cur_node, bool print_in_pdf);

bool IsFictiveZero (Operations op);

TreeNode* CreateDigitNode (double dbl_val);

int SimplifyTree (TreeNode* cur_node);

bool isZero (double num);

bool isEqual (double num1, double num2);

double CalcTree (TreeNode* node, double val);

double CalcOneOp (double val1, double val2, Operations operation);

TreeNode* GetOperationNode (TreeNode* child_node, Operations op);

void FillTokensArray (Token* token_array);

Token CreateToken (Types type, double dbl_val, Operations op_t, int line_number);

void PrintTokens (Token token_array[]);

#endif