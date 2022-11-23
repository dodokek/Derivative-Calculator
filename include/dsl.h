#ifndef DSL_H
#define DSL_H

#define DL GetDerivative (cur_node->left, root)
#define DR GetDerivative (cur_node->right, root)
#define CL CopyNode (cur_node->left)
#define CR CopyNode (cur_node->right)
#define GET_DIGIT(num) CreateDigitNode (num)

#define ADD(L, R) CreateNode (OP_T, 0, ADD, nullptr, L, R)
#define SUB(L, R) CreateNode (OP_T, 0, SUB, nullptr, L, R)
#define MUL(L, R) CreateNode (OP_T, 0, MUL, nullptr, L, R)
#define DIV(L, R) CreateNode (OP_T, 0, DIV, nullptr, L, R)
#define SIN(L, R) CreateNode (OP_T, 0, SIN, nullptr, L, R)
#define COS(L, R) CreateNode (OP_T, 0, COS, nullptr, L, R)
#define LN(L, R) CreateNode (OP_T, 0, LN, nullptr, L, R)


#define INIT_PARAMS OP_T, 0, UNKNOWN, nullptr, nullptr, nullptr

#define $PRINT_N_RETURN(X)       \
    {                            \
        TreeNode* tmp_node = X;  \
        PrintBranch (tmp_node);  \
        return tmp_node;         \
    }

#endif