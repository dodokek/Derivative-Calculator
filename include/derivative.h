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


//-----------------------------------------------

enum Positions
{
    LEFT,
    RIGHT,
};

enum Answers
{
    NO,
    YES,
};


enum Sizes
{
    MAX_IMG_SRC_LEN = 100,
    STACK_INIT_CAP = 5,
    MAX_NAME_LEN = 100,
    MAX_MESSAGE_LEN =1000,
};


enum PlayMode
{
    EXIT = 0,
    GUESS = 1,
    LISTING = 2,
    COMPARISON = 3,
};

const int OFFSET = 2;

//-----------------------------------------------


struct node
{
    node* parent;

    node* left;
    node* right;
    // 

    char* name;
};


#define $YES                                                                        \
    mciSendString("open \"data/yes.mp3\" type mpegvideo alias mp3", NULL, 0, NULL); \
    mciSendString("play mp3", NULL, 0, NULL);                                       


bool isNegativeAns (node* cur_node);

node* GetNodeFromUser (node* root);

node* GetTreeRoot ();

node* InsertNode (char name[], node* parent, Positions position = LEFT);

node* CreateNewNode ();

node* InitTreeRoot (char name[]);

char* GetInput (char* buffer);

void AddNode (node* cur_node);

void DumpTree (node* root);

void DrawTree (node* root);

void PrintPreOrder (node* node, FILE* tree_data);

void PrintInOrder (node* node, FILE* tree_data);

void PrintPostOrder (node* node, FILE* tree_data);

void PrintObject (node* node_to_print);

Stack BuildAncestorsStack (node* cur_node);

void AddAncestor (node* cur_node, Stack* ancestors);

void InitGraphvisNode (node* node, FILE* dot_file);

void RecursDrawConnections (node* node, FILE* dot_file);

node* FindNode (node* cur_node, const char name[]);

node* BuildTree (FILE* tree_info);

void AddRightChild (node* cur_node);

void AddLeftChild (node* cur_node);

int FillCurrNode(node* currnode, char* buffer);

node* DestructTree (node* root);



#endif