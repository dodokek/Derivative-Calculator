#ifndef FRONT_H
#define FRONT_H

#include "backend.h"

void PrintInFile (TreeNode* root);

void DumpTree (TreeNode* root);

void DrawTree (TreeNode* root);

void PrintInOrder (TreeNode* node, FILE* out_file);

bool isNeedDivision (TreeNode* op_node);

char* GetOpSign (Operations op);

void InitGraphvisNode (TreeNode* node, FILE* dot_file);

void RecursDrawConnections (TreeNode* node, FILE* dot_file);

TreeNode* FindNode (TreeNode* cur_node, const char name[]);

#endif