#define DL GetDerivative (cur_node->left)
#define DR GetDerivative (cur_node->right)
#define CL CopyNode (cur_node->left)
#define CR CopyNode (cur_node->right)

#define ADD(L, R) CreateNode (OP_T, 0, ADD, nullptr, L, R)
#define SUB(L, R) CreateNode (OP_T, 0, SUB, nullptr, L, R)
#define MUL(L, R) CreateNode (OP_T, 0, MUL, nullptr, L, R)