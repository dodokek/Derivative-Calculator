#ifndef FRONT_H
#define FRONT_H

#include "backend.h"
#include "dsl.h"


enum PrintTypes
{
    ORIGIN,
    DERIVATIVE,
    INFO,
    RESULT,
};

void InitLatexFile (TreeNode* root);

void GeneratePdf ();

void PrintBranch (TreeNode* root, PrintTypes mode);

void DumpTree (TreeNode* root);

void DrawTree (TreeNode* root);

void PrintInOrder (TreeNode* node, FILE* out_file);

bool isNeedDivision (TreeNode* op_node);

char* GetOpSign (Operations op);

void InitGraphvisNode (TreeNode* node, FILE* dot_file);

void RecursDrawConnections (TreeNode* node, FILE* dot_file);

TreeNode* FindNode (TreeNode* cur_node, const char name[]);

void CalcFuncInPoint (FILE* out_file, TreeNode* root);

void PrintGraphic (TreeNode* root, int begin_x, int end_x, FILE* out_file);

void PrintMacloren (TreeNode* root, int steps);

#endif